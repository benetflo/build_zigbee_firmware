/**
  ******************************************************************************
  * @file           fsm.c
  * @brief          Implementation file for Finite State Machine functions and definitions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 SEACOMP. All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component. If no LICENSE file is provided,
  * the software is provided AS-IS.
  *
  ******************************************************************************
  */


#include "led_fsm.h"
#include "power_fsm.h"
#include "stm32c0xx_ll_utils.h"
#include "auxiliar_functions.h"
#include "handlers.h"
#include <stdbool.h>
#include "SDK_systick.h"
#include "SDK_gpio.h"
#include "SDK_adc.h"
#include "SDK_pwm.h"
#include "button.h"
#include "regFile.h"
#include "global_state.h"
#include "main.h"

powerState_t currentPowerState = STATE_POWER_INITIALIZATION;
powerState_t nextPowerState =  STATE_MAIN_POWER_FAILURE;
uint32_t bootTimer = 0;
uint16_t t_MAIN_STABLE = 500;
uint16_t t_UPS_CHARGERESUME = 500;
uint8_t mainPowerOnFlag = false;

static uint16_t voltageBuffer[4];

void enableMainPower(void);
void enableGlobalEnableRpi(void);
void disableMainPower(void);
void disableGlobalEnableRpi(void);
void enableUpsBoost(void);
void disableUpsBoost(void);
void enableUpsLong(void);
void disableUpsLong(void);
void enableCharging(void);
void disableCharging(void);
void enableBatteryMeasure(void);
void disableBatteryMeasure(void);

void updateBatteryFsm(void);
void updateADCreadings(void);
uint16_t getMainPowerVoltage();

batteryState_t currentBatteryState = BATTERY_INIT_STATE;
batteryState_t nextBatteryState = BATTERY_INIT_STATE;

uint8_t btnStateInc = 0;

void shutdownActions() {
  uint8_t regValue = 0;
  reg_write(REGFILE_OFFSET_PERIPHERAL_CONFIG, &regValue, 1);
  reg_write(REGFILE_OFFSET_RPI_STATE, &regValue, 1);
  disableGlobalEnableRpi();
  disableMainPower();
  disableUpsBoost();
  disableUpsLong();
  //btnStateInc = 0;
}

void handleButtonPress_(void) {
  static uint32_t buttonPressedTimer = 0;
  static uint32_t buttonReleasedTimer = 0;
  static uint32_t shutdownDelayTimer = 0;
  const uint32_t LONG_PRESS_THRESHOLD = 5000;  // 5 seconds for forced reset
  const uint32_t SHUTDOWN_DELAY_MS = 30000;     // 30 seconds delay before shutdown

  if (isButtonPressed()) {
    buttonPressedTimer = GetmsTicks();
  }

  if (isButtonReleased()) {
    buttonReleasedTimer = GetmsTicks();
  }

  if ((buttonPressedTimer != 0) && (buttonReleasedTimer != 0)) {
    uint32_t pressDuration = buttonReleasedTimer - buttonPressedTimer;

    if (pressDuration > LONG_PRESS_THRESHOLD && getGlobalState() != MAINTENANCE_STATE) {
      // Long press - Immediate forced reset
      shutdownActions();
      resetGlobalState();
      setGlobalState(INIT_STATE);
      nextPowerState = STATE_POWER_INITIALIZATION;

    } else if (getGlobalState() != MAINTENANCE_STATE || (pressDuration > LONG_PRESS_THRESHOLD && getGlobalState() != MAINTENANCE_STATE)) {
      if (currentPowerState != STATE_MAIN_SHUTDOWN) {
        uint8_t rpiState = 2;
        reg_write(REGFILE_OFFSET_RPI_STATE, &rpiState, 1);
        resetGlobalState();
        setGlobalState(SHUTDOWN_STATE);
        nextPowerState = STATE_MAIN_SHUTDOWN;
        btnStateInc = 10;

        // Start the shutdown delay timer (non-blocking shutdown trigger)
        shutdownDelayTimer = GetmsTicks();
      } else {
        // If already in shutdown state, cancel shutdown and re-init
        if (nextPowerState == currentPowerState) {
          keepButtonReleased();
        }
        resetGlobalState();
        setGlobalState(INIT_STATE);
        nextPowerState = STATE_INIT;
      }
    }
    else{ // If wer are in maintenance, but the click was short, then we switch the mode.
    	btnStateInc++;
    	if(btnStateInc == 2)
    		btnStateInc = 0;
    	reg_write(REGFILE_METRICS_ISYS_LOW,&btnStateInc,1);
//    	regFile[REGFILE_METRICS_ISYS_LOW].value = btnStateInc;

    }

    // Reset button press/release timers
    buttonPressedTimer = 0;
    buttonReleasedTimer = 0;
    currentPowerState = nextPowerState;
  }

  // Delayed shutdown handling (non-blocking)
  if (currentPowerState == STATE_MAIN_SHUTDOWN && shutdownDelayTimer > 0) {
    if (GetmsTicks() - shutdownDelayTimer >= SHUTDOWN_DELAY_MS) {
      shutdownActions();
      disableMainPower();
      shutdownDelayTimer = 0;  // Clear the timer after shutdown
    }
  }
}

void handleButtonPress(void) {
  static uint32_t buttonPressedTimer = 0;
  static uint32_t buttonReleasedTimer = 0;
  static uint32_t shutdownDelayTimer = 0;
  const uint32_t LONG_PRESS_THRESHOLD = 5000;  // 5 seconds for forced reset
  const uint32_t SHUTDOWN_DELAY_MS = 30000;     // 30 seconds delay before shutdown

  if (isButtonPressed()) {
    buttonPressedTimer = GetmsTicks();
  }

  if (isButtonReleased()) {
    buttonReleasedTimer = GetmsTicks();
  }

  if ((buttonPressedTimer != 0) && (buttonReleasedTimer != 0)) {
    uint32_t pressDuration = buttonReleasedTimer - buttonPressedTimer;

    if (pressDuration > LONG_PRESS_THRESHOLD) {

    	if (currentPowerState != STATE_MAIN_SHUTDOWN) {
        uint8_t rpiState = 2;
        reg_write(REGFILE_OFFSET_RPI_STATE, &rpiState, 1);
        resetGlobalState();
        setGlobalState(SHUTDOWN_STATE);
        nextPowerState = STATE_MAIN_SHUTDOWN;
        btnStateInc = 10;

        // Start the shutdown delay timer (non-blocking shutdown trigger)
        shutdownDelayTimer = GetmsTicks();
      } else {
        // If already in shutdown state, cancel shutdown and re-init
        if (nextPowerState == currentPowerState) {
          keepButtonReleased();
        }
        resetGlobalState();
        setGlobalState(INIT_STATE);
        nextPowerState = STATE_INIT;
        btnStateInc = 0;
      }
    }
    else{ // If we are in maintenance, but the click was short, then we switch the mode.
    	btnStateInc++;
    	if(btnStateInc == 2)
    		btnStateInc = 0;
//    	regFile[REGFILE_METRICS_ISYS_LOW].value = btnStateInc;

    }
	reg_write(REGFILE_METRICS_ISYS_LOW,&btnStateInc,1);

    // Reset button press/release timers
    buttonPressedTimer = 0;
    buttonReleasedTimer = 0;
    currentPowerState = nextPowerState;
  }

  // Delayed shutdown handling (non-blocking)
  if (currentPowerState == STATE_MAIN_SHUTDOWN && shutdownDelayTimer > 0) {
    if (GetmsTicks() - shutdownDelayTimer >= SHUTDOWN_DELAY_MS) {
      shutdownActions();
      disableMainPower();
      shutdownDelayTimer = 0;  // Clear the timer after shutdown
    }
  }
}


void updateBatteryFsm() {
  static uint32_t stateTimer = 0;

  switch (currentBatteryState) {
    case BATTERY_CHARGING_STATE:
      if (getMainPowerVoltage() < V_UPS_TURNOFF) {
        disableCharging();
        clearGlobalState(CHARGE_EN_STATE);
        nextBatteryState = BATTERY_INIT_STATE;
      } else {
        nextBatteryState = BATTERY_CHARGING_STATE;
      }
      break;
    case BATTERY_INIT_STATE:
      if (getMainPowerVoltage() < V_UPS_TURNON) {
        // If the voltage is insufficient, stay off without indication

        nextBatteryState = BATTERY_INIT_STATE;
        stateTimer = GetmsTicks();
        break;
      }
      if ((GetmsTicks() - stateTimer) >= t_UPS_CHARGERESUME) {
    	enableCharging();
    	setGlobalState(CHARGE_EN_STATE);
        stateTimer = GetmsTicks();    // Reset timer for the next state
        nextBatteryState = BATTERY_CHARGING_STATE; // Proceed to the next state
      } else {
    	// continue monitoring
    	nextBatteryState = BATTERY_INIT_STATE;
      }
      break;
  }
  currentBatteryState = nextBatteryState;
}

void updatePowerFsm() {
  static uint32_t stateTimer = 0;
  updateADCreadings();
  updateBatteryFsm();
  handleButtonPress();
  switch (currentPowerState) {
    case STATE_INIT:
      stateTimer = GetmsTicks();
      if (isButtonReleased()) {
        nextPowerState = STATE_POWER_INITIALIZATION;
      }
      break;

    case STATE_POWER_INITIALIZATION:
      // Start monitoring the main power source voltage
      if (getMainPowerVoltage() < V_MAIN_TURNON) {
        // If the voltage is insufficient, stay off without indication
    	  nextPowerState = STATE_POWER_INITIALIZATION;
    	  stateTimer = GetmsTicks();
        break;
      }

      // Check if the voltage is stable for the required time
      if ((GetmsTicks() - stateTimer) >= t_MAIN_STABLE) {
        // Voltage has been stable for the required time, enable main power
        enableMainPower();
        enableGlobalEnableRpi();
        enableUpsBoost();
        stateTimer = GetmsTicks();    // Reset timer for the next state
        nextPowerState = STATE_MAIN_POWER_ON; // Proceed to the next state
        clearGlobalState(INIT_STATE);
      } else {
        // Continue monitoring
    	nextPowerState = STATE_POWER_INITIALIZATION;
      }
      break;
    case STATE_MAIN_POWER_ON:
      if (getMainPowerVoltage() < V_MAIN_TURNOFF) {
    	disableMainPower();
    	enableUpsLong();

        stateTimer = GetmsTicks();
        nextPowerState = STATE_MAIN_POWER_FAILURE;
        setGlobalState(ERR_FLAG_STATE | BATT_PWR_ON_STATE);
      }
      break;

    case STATE_MAIN_POWER_FAILURE:
      if (getMainPowerVoltage() < V_MAIN_TURNON) {
        nextPowerState = STATE_MAIN_POWER_FAILURE;
        stateTimer = GetmsTicks();
        break;
      }
      // Check if the voltage is stable for the required time
      if ((GetmsTicks() - stateTimer) >= t_MAIN_STABLE) {
        // Voltage has been stable for the required time, enable main power
    	disableUpsLong();
    	enableMainPower();
        stateTimer = GetmsTicks();
        nextPowerState = STATE_MAIN_POWER_ON;
        clearGlobalState(ERR_FLAG_STATE | BATT_PWR_ON_STATE);
      } else {
     	nextPowerState = STATE_MAIN_POWER_FAILURE;
      }
      break;
    case STATE_MAIN_SHUTDOWN:
      break;

  }
  currentPowerState = nextPowerState;
}

void updateADCreadings() {
  SDK_ADC_GetConversionResults(voltageBuffer, sizeof(voltageBuffer));
  voltageBuffer[0] = SDK_ADC_calculateVin(voltageBuffer[0], 100000, 330000); //battery
  voltageBuffer[1] = SDK_ADC_calculateVin(voltageBuffer[1], 100000, 30000); //vusb
  voltageBuffer[2] = SDK_ADC_calculateVin(voltageBuffer[2], 100000, 30000); //vsys
}

void enableMainPower() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_12, GPIO_PIN_HIGH);
}

void disableMainPower() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_12, GPIO_PIN_LOW);
}

void enableGlobalEnableRpi() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_15, GPIO_PIN_LOW);
}

void disableGlobalEnableRpi() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_15, GPIO_PIN_HIGH);
}

void enableUpsBoost() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_8, GPIO_PIN_HIGH);
}

void disableUpsBoost() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_8, GPIO_PIN_LOW);
}

void enableCharging() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_4, GPIO_PIN_LOW);
}

void disableCharging() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_4, GPIO_PIN_HIGH);
}

void enableUpsLong() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_7, GPIO_PIN_HIGH);
}

void disableUpsLong() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_7, GPIO_PIN_LOW);
}

void enableBatteryMeasure() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_6, GPIO_PIN_LOW);
}

void disableBatteryMeasure() {
  SDK_GPIO_WritePin(GPIOA, LL_GPIO_PIN_6, GPIO_PIN_HIGH);
}

uint16_t getBatteryVoltage() {
  return voltageBuffer[0];
}

uint16_t getMainPowerVoltage() {
  return voltageBuffer[1];
}

uint16_t getVsysVoltage() {
  return voltageBuffer[2];
}
