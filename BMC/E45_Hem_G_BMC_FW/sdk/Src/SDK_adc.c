#include "stm32c0xx_ll_adc.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_utils.h"
#include "SDK_tim.h"
#include "SDK_systick.h"
#include "SDK_adc.h"

#define MAX_ADC_CHANNELS 5

#define VREFINT_CAL      (*((uint16_t*)VREFINT_CAL_ADDR))

static volatile uint16_t adcBuffer1[MAX_ADC_CHANNELS];
static volatile uint16_t adcBuffer2[MAX_ADC_CHANNELS];
static volatile uint16_t *writeBuffer = adcBuffer1;
static volatile uint16_t *readBuffer = adcBuffer2;

static volatile uint16_t adcIndex = 0;
static volatile uint8_t adcConversionComplete = 0;

uint16_t SDK_ADC_VREFINTFactoryCalibrationValue(void) {
  return VREFINT_CAL;
}

void SDK_ADC_InitInterruptMode(SDK_ADC_ChannelConfig *channelConfigs, uint8_t numChannels) {
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP2_EnableClock(LL_APB2_GRP1_PERIPH_ADC);

  /* ADC1 interrupt Init */
  LL_ADC_EnableIT_EOC(ADC1);
  NVIC_EnableIRQ(ADC1_IRQn);
  NVIC_SetPriority(ADC1_IRQn, 3);

  /** Common config */
  LL_ADC_SetResolution(ADC1, LL_ADC_RESOLUTION_12B);
  LL_ADC_SetDataAlignment(ADC1, LL_ADC_DATA_ALIGN_RIGHT);
  LL_ADC_REG_SetSequencerConfigurable(ADC1, LL_ADC_REG_SEQ_CONFIGURABLE);

  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = (0xFUL << (4U * numChannels));//sequencerLengths[numChannels];
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_1RANK;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_SetClock(ADC1, LL_ADC_CLOCK_SYNC_PCLK_DIV4);

  /* Configure Regular Channel */
  for (uint8_t rank = 0; rank < numChannels; rank++) {
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1 + (rank << 2), channelConfigs[rank].channel);
    LL_ADC_SetChannelSamplingTime(ADC1, channelConfigs[rank].channel, channelConfigs[rank].samplingTime);

    if (channelConfigs[rank].channel == LL_ADC_CHANNEL_TEMPSENSOR) {
      LL_ADC_SetCommonPathInternalCh(ADC1_COMMON, LL_ADC_PATH_INTERNAL_TEMPSENSOR);
    } else if (channelConfigs[rank].channel == LL_ADC_CHANNEL_VREFINT) {
      LL_ADC_SetCommonPathInternalCh(ADC1_COMMON, LL_ADC_PATH_INTERNAL_VREFINT);
    }
  }

  // Calibration and enabling
  if (LL_ADC_IsEnabled(ADC1)) {
    LL_ADC_Disable(ADC1);
    while (LL_ADC_IsEnabled(ADC1));
  }

  LL_ADC_EnableInternalRegulator(ADC1);
  LL_mDelay(10);

  LL_ADC_StartCalibration(ADC1);
  while (LL_ADC_IsCalibrationOnGoing(ADC1));

  LL_ADC_Enable(ADC1);
  while (!LL_ADC_IsActiveFlag_ADRDY(ADC1));

  SDK_ADC_StartConversion();
}

void SDK_ADC_StartConversion(void) {
  LL_ADC_REG_StartConversion(ADC1);
}

uint8_t SDK_ADC_IsConversionComplete(void) {
  return adcConversionComplete;
}

void SDK_ADC_GetConversionResults(uint16_t *buffer, uint16_t size) {
  LL_ADC_DisableIT_EOC(ADC1);
  NVIC_DisableIRQ(ADC1_IRQn);
  //uint16_t vdda = (uint16_t)((VREFINT_CAL_VREF * VREFINT_CAL) / adcBuffer[size - 1]);
  uint16_t vdda = 3300;

  // Calibrate each ADC result (excluding the last one)
  for (uint16_t i = 0; i < size - 1; i++) {
    buffer[i] = (uint16_t)((readBuffer[i] * vdda) / 4096);
  }
  buffer[size - 1] = readBuffer[size - 1];
  LL_ADC_EnableIT_EOC(ADC1);
  NVIC_EnableIRQ(ADC1_IRQn);
  NVIC_SetPriority(ADC1_IRQn, 3);

}

uint16_t SDK_ADC_calculateVin(uint16_t vout, uint32_t r1, uint32_t r2) {
  if (r2 == 0) {
    return 0; // Return an error value (0 indicates invalid result)
  }
  return (vout * (r1 + r2)) / r2;
}

void ADC1_IRQHandler(void) {
  if (LL_ADC_IsActiveFlag_EOC(ADC1)) {
    /* Clear EOC flag and store ADC data */
    LL_ADC_ClearFlag_EOC(ADC1);
    writeBuffer[adcIndex++] = LL_ADC_REG_ReadConversionData12(ADC1);

    if (adcIndex >= MAX_ADC_CHANNELS) {
      /* All channels converted */
      adcIndex = 0;

      // Swap buffers
      volatile uint16_t *temp = readBuffer;
      readBuffer = writeBuffer;
      writeBuffer = temp;

      adcConversionComplete = 1;
    }
    LL_ADC_REG_StartConversion(ADC1);
  }
}
