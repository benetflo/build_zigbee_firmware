/*
 * button.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Dell
 */

#include "button.h"
#include "SDK_systick.h"
#include "stm32c0xx_ll_exti.h"
#include <stdint.h>

#define DEBOUNCE_INTERVAL_MS 25

typedef enum {
	RELEASED =0,
	PRESSED,
	IDLE
} buttonEvent_t;

static volatile buttonEvent_t buttonEvent = IDLE;
static volatile uint32_t lastDebounceTime = 0;

uint8_t isButtonPressed() {
  if (buttonEvent == PRESSED) {
    buttonEvent = IDLE;
    return 1;
  }
  return 0;
}

uint8_t isButtonReleased(){
  if (buttonEvent == RELEASED) {
    buttonEvent = IDLE;
    return 1;
  }
  return 0;
}

void keepButtonReleased() {
	buttonEvent = RELEASED;
}

void EXTI4_15_IRQHandler(void){
  uint32_t currentTime = GetmsTicks();

  if (LL_EXTI_IsActiveFallingFlag_0_31(LL_EXTI_LINE_5) != RESET) {
    LL_EXTI_ClearFallingFlag_0_31(LL_EXTI_LINE_5);
    if ((currentTime - lastDebounceTime) > DEBOUNCE_INTERVAL_MS) {
      buttonEvent = RELEASED;
      lastDebounceTime = currentTime;
    }
  }
  if (LL_EXTI_IsActiveRisingFlag_0_31(LL_EXTI_LINE_5) != RESET) {
    LL_EXTI_ClearRisingFlag_0_31(LL_EXTI_LINE_5);
    if ((currentTime - lastDebounceTime) > DEBOUNCE_INTERVAL_MS) {
      buttonEvent = PRESSED;
      lastDebounceTime = currentTime;
    }
  }
}
