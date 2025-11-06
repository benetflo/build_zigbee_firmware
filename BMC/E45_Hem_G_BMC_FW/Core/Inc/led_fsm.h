/*
 * ledFsm.h
 *
 *  Created on: Nov 30, 2024
 *      Author: Dell
 */

#ifndef _LEDFSM_H_
#define _LEDFSM_H_

#include <stdint.h>

void LedSwirl(void);
void LedsToGreen(void);
void LedsToPulsingBlue(void);
void LedsToBlinkingYellow(void);
void LedsToFlashingRed(void);
void updateLedState(void);

#endif /* INC_LEDFSM_H_ */
