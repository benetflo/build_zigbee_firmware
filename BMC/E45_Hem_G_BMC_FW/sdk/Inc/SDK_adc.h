#ifndef _SDK_ADC_H_
#define _SDK_ADC_H_

#include "stm32c0xx_ll_adc.h"

#define ADC_SAMPLINGTIME_07CYCLES_5		(0b010)
#define ADC_SAMPLINGTIME_19CYCLES_5		(0b100)
#define ADC_SAMPLINGTIME_79CYCLES_5		(0b110)

typedef struct {
  uint32_t channel;
  uint32_t samplingTime;
} SDK_ADC_ChannelConfig;

uint16_t SDK_ADC_VREFINTFactoryCalibrationValue();
void SDK_ADC_InitInterruptMode(SDK_ADC_ChannelConfig *channelConfigs, uint8_t numChannels);
void SDK_ADC_GetConversionResults(uint16_t *buffer, uint16_t size);
uint8_t SDK_ADC_IsConversionComplete(void);
uint16_t SDK_ADC_ReadValue(void);
uint16_t SDK_ADC_calculateVin(uint16_t vout, uint32_t r1, uint32_t r2);
void SDK_ADC_StartConversion(void);

#endif /* _SDK_ADC_H_ */
