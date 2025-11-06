/*
 * global_state.h
 *
 *  Created on: Jan 16, 2025
 *      Author: rogale
 */

#ifndef _GLOBAL_STATE_H_
#define _GLOBAL_STATE_H_

#include <stdint.h>

// Define the states as bit flags
#define SHUTDOWN_STATE     (1 << 6)
#define INIT_STATE         (1 << 5)
#define STANDBY_STATE      (1 << 4)
#define MAINTENANCE_STATE  (1 << 3)
#define CHARGE_EN_STATE    (1 << 2)
#define BATT_PWR_ON_STATE  (1 << 1)
#define ERR_FLAG_STATE     (1 << 0)

// Functions to manipulate the state
void setGlobalState(uint8_t state);
void resetGlobalState();
uint8_t getGlobalState();
void clearGlobalState(uint8_t state);
uint8_t isGlobalStateSet(uint8_t state);
uint8_t rpiState(void);

#endif /* _GLOBAL_STATE_H_ */
