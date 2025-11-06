/*
 * regFile.c
 *
 *  Created on: Nov 15, 2024
 *      Author: jurber
 */

#include <string.h>
#include <SDK_uart.h>
#include "SDK_gpio.h"
#include "SDK_pwm.h"
#include "SDK_adc.h"
#include "regFile.h"
#include "comms.h"
#include "ringBuf.h"

#include "main.h"

#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_usart.h"

typedef union {
    const uint8_t *ptr;
    uint8_t value;
} RegEntry_t;

static RegEntry_t regFile[REGFILE_SIZE] = {0};

__attribute__ ((section(".PN"))) const uint8_t HW_PN[16] = {"41324E4F47505301"}; ///< Hardware part number
__attribute__ ((section(".SN"))) const uint8_t HW_SN[16] = {"SERIAL_NUMBER___"}; ///< Hardware serial number
__attribute__ ((section(".FW"))) const uint8_t HW_FW[52] = {"FIRMWARE_VERSION____________________________________"}; ///< Firmware version
__attribute__ ((section(".HW"))) const uint8_t HW_HW[16] = {"HARDWARE_VERSION"}; ///< Hardware version/revision

// This function initializes the register file
void reg_init(void){
  // TODO: finish function
  // Numbering and logistics placeholders, to be replaced by post-build-action in object file

  regFile[0].ptr = HW_PN;
  regFile[1].ptr = HW_SN;
  regFile[2].ptr = HW_FW;
  regFile[3].ptr = HW_HW;

  regFile[REGFILE_OFFSET_MAXPAYLOAD].value = COMMS_PACKET_MAXLENGTH;
  regFile[REGFILE_OFFSET_PROTOCOL].value = 0xFF;		// CUSTOM, not maintained by library or repo
}

// This function is customer per implementation and dictates which registers to track for changes
void reg_trackChanges(void){
  reg_CONFIG_handler();
  reg_OpMetrics_handler();
}

// This function performs a register-read and copies the data to a destination array.
void reg_read(uint8_t* dest, uint8_t address, uint8_t length) {
  // Overflow condition
  if(address + length > REGFILE_SIZE){
    length = REGFILE_SIZE - address;
  }

  switch (address) {
  case 0:
  case 1:
  case 2:
  case 3:
	  memcpy(dest, regFile[address].ptr, length);
	break;
  default:
	// Perform copy
	for(uint8_t n=0; n<length; n++){
	  *(dest+n) = regFile[address+n].value;
	}
	break;
  }
}

// TODO: implement a mask to have true read-only registers
// This function performs an unmasked register-write.
uint8_t reg_write(uint8_t address, uint8_t* src, uint8_t length) {
  uint8_t n;

  // Overflow condition
  if(address + length > REGFILE_SIZE){
    length = REGFILE_SIZE - address;
  }

  // Perform copy
  for(n=0; n<length; n++){
    regFile[address+n].value = *(src+n);
  }
  return length;
}

// ====================================================================
// Register checking

void reg_CONFIG_handler(void) {
  static uint8_t CONFIG_old;

  if(CONFIG_old == regFile[REGFILE_OFFSET_PERIPHERAL_CONFIG].value){
	return;
  }

  if(regFile[REGFILE_OFFSET_PERIPHERAL_CONFIG].value & REGFILE_CONFIG_SIREN_EN){
    //TODO: set global variable to start the siren (or we start here?)
	SDK_PWM_Start();
  } else {
	SDK_PWM_Stop();
  }
  // Store for next cycle
  CONFIG_old = regFile[REGFILE_OFFSET_PERIPHERAL_CONFIG].value;
}

void reg_QUECTEL_handler(void) {
  static uint8_t QUECTEL_old;

  if(QUECTEL_old == regFile[REGFILE_QUECTEL_CONFIG].value){
	return;
  }

  if(regFile[REGFILE_QUECTEL_CONFIG].value & REGFILE_QUECTEL_AP_READY){
	SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_0, GPIO_PIN_HIGH);
  } else {
	SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_0, GPIO_PIN_LOW);
  }

  if(regFile[REGFILE_QUECTEL_CONFIG].value & REGFILE_QUECTEL_PSM_EINT){
    SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_3, GPIO_PIN_HIGH);
  } else {
    SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_3, GPIO_PIN_LOW);
  }

  if(regFile[REGFILE_QUECTEL_CONFIG].value & REGFILE_QUECTEL_RESET_n){
	SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_4, GPIO_PIN_LOW);
  } else {
	SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_4, GPIO_PIN_HIGH);
  }

  if(regFile[REGFILE_QUECTEL_CONFIG].value & REGFILE_QUECTEL_USB_BOOT){
    SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_1, GPIO_PIN_HIGH);
  } else {
    SDK_GPIO_WritePin(GPIOB, LL_GPIO_PIN_1, GPIO_PIN_LOW);
  }

  // Store for next cycle
  QUECTEL_old = regFile[REGFILE_QUECTEL_CONFIG].value;
}

void reg_OpMetrics_handler(void) {
  uint16_t voltageBuffer[4];
  SDK_ADC_GetConversionResults(voltageBuffer, sizeof(voltageBuffer));

  uint16_t temp = SDK_ADC_calculateVin(voltageBuffer[1], 100000, 30000);
  regFile[REGFILE_METRICS_VUSB_LOW].value = temp & 0xff;
  regFile[REGFILE_METRICS_VUSB_HIGH].value = (temp >> 8) & 0xff;

  temp = SDK_ADC_calculateVin(voltageBuffer[2], 100000, 30000);
  regFile[REGFILE_METRICS_VSYS_LOW].value = temp & 0xff;
  regFile[REGFILE_METRICS_VSYS_HIGH].value = (temp >> 8) & 0xff;

  temp = SDK_ADC_calculateVin(voltageBuffer[0], 100000, 330000);
  regFile[REGFILE_METRICS_VBAT_LOW].value = temp & 0xff;
  regFile[REGFILE_METRICS_VBAT_HIGH].value = (temp >> 8) & 0xff;
//  regFile[REGFILE_METRICS_ISYS_LOW].value = 0;
//  regFile[REGFILE_METRICS_ISYS_HIGH].value = 0;

  int32_t tempC = __LL_ADC_CALC_TEMPERATURE_TYP_PARAMS(
      2530,
      760,
      30,
      3300,
      voltageBuffer[3],
      LL_ADC_RESOLUTION_12B
  );

  regFile[REGFILE_METRICS_TEMP_MSB].value = (tempC >> 24) & 0xff;
  regFile[REGFILE_METRICS_TEMP_SECOND_BYTE].value = (tempC >> 16) & 0xff;
  regFile[REGFILE_METRICS_TEMP_THIRD_BYTE].value = (tempC >> 8) & 0xff;
  regFile[REGFILE_METRICS_TEMP_LSB].value = tempC & 0xff;
}

void reg_CONFIG2_handler(void){}
