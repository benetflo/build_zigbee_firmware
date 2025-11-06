/**
  ******************************************************************************
  * @file           fsm.h
  * @brief          Header file for Finite State Machine functions and definitions
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

#ifndef _FSM_H_
#define _FSM_H_

#include <stdint.h>

// Constants for time thresholds and voltage thresholds
#define V_MAIN_TURNON			     11000
#define V_MAIN_TURNOFF			     10000
#define V_UPS_TURNON			     V_MAIN_TURNON
#define V_UPS_TURNOFF			     V_MAIN_TURNOFF
#define UPS_DISSIPATION_DELAY         500     // Delay for enabling UPS_LONG_EN to reduce diode dissipation
#define MAIN_POWER_HOLDOFF_TIME      1000     // Hold-off time after power restoration
#define UPS_CHARGE_RESUME_DELAY      2000     // Delay to resume charging after power restore
#define VSYS_EXPECTED_VOLTAGE		11500     // Voltage expected in VSYS


typedef enum {
  STATE_INIT,
  STATE_POWER_INITIALIZATION,
  STATE_MAIN_POWER_ON,
  STATE_MAIN_POWER_FAILURE,
  STATE_MAIN_SHUTDOWN
} powerState_t;

typedef enum {
  BATTERY_INIT_STATE,
  BATTERY_CHARGING_STATE
} batteryState_t;

void updatePowerFsm(void);

#endif /* _FSM_H_ */
