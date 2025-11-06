/*
 * leds.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Dell
 */

#ifndef _LEDS_H_
#define _LEDS_H_

#include <stdint.h>

#define LED_COUNT 1
#define REG_COLOR_LEDS (LED_COUNT * 3)  // 9 LEDs * 3 (RGB)

uint32_t GenerateColorCommand(uint8_t flag, uint8_t dimming, uint8_t blue, uint8_t green, uint8_t red);
void SendColorCommandArray(uint8_t *data, uint16_t size);

#endif /* DRIVERS_LEDS_INC_LEDS_H_ */
