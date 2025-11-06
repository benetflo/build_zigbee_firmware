/*
 * SDK_pwm.c
 *
 *  Created on: Dec 3, 2024
 *      Author: Dell
 */


#include "stm32c0xx_ll_tim.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_system.h"

void SDK_PWM_Init(void) {
  //TODO: Split this function to be modular between setup and the actual configuration.
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_5, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_5, LL_GPIO_PULL_NO);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_5, LL_GPIO_SPEED_FREQ_HIGH);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_5, LL_GPIO_AF_5);

  /* Enable TIM1 clock */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_TIM1);

  /* Configure TIM1 in PWM mode for Channel 2 */
  LL_TIM_SetCounterMode(TIM1, LL_TIM_COUNTERMODE_UP);
  LL_TIM_SetPrescaler(TIM1, 10);        // Prescaler: Timer clock = 48 MHz / (479 + 1) = 100 kHz
  LL_TIM_SetAutoReload(TIM1, 969);       // 956-969 Auto-reload: PWM frequency = 100 kHz / (999 + 1) = 100 Hz
  LL_TIM_OC_SetMode(TIM1, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
  LL_TIM_OC_SetCompareCH1(TIM1, 485);    // 478-485Compare value: 50% duty cycle (500 / 1000)
  LL_TIM_OC_EnablePreload(TIM1, LL_TIM_CHANNEL_CH1);

  LL_TIM_CC_EnableChannel(TIM1, LL_TIM_CHANNEL_CH1);
}

void SDK_PWM_Start(void) {
  //static uint8_t triggered = 0;
  //if (!triggered) {
  LL_TIM_EnableCounter(TIM1);
  LL_TIM_EnableAutomaticOutput(TIM1);
  //triggered = 1;
  //}
}

void SDK_PWM_Stop(void) {
  LL_TIM_DisableCounter(TIM1);
  LL_TIM_DisableAutomaticOutput(TIM1);
}
