#include "SDK_tim.h"

static volatile uint32_t TimerOverflows = 0;

//TODO: Check if I can split this initialization function in two: Timer itself and Channel configuration.
void SDK_TIM_InputCapture_Init(SDK_TIM_InitTypeDef *Config) {
  // Enable the clock for the selected timer and GPIO port
  if (Config->TIMx == TIM1) LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_TIM1);
  else if (Config->TIMx == TIM3) LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  // Configure GPIO pin for alternate function
  LL_GPIO_SetPinMode(Config->GPIOx, Config->GPIO_Pin, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(Config->GPIOx, Config->GPIO_Pin, Config->GPIO_AF);
  LL_GPIO_SetPinPull(Config->GPIOx, Config->GPIO_Pin, LL_GPIO_PULL_NO);

  // Configure timer input capture
  LL_TIM_IC_SetActiveInput(Config->TIMx, Config->Channel, LL_TIM_ACTIVEINPUT_DIRECTTI);
  LL_TIM_IC_SetPrescaler(Config->TIMx, Config->Channel, Config->IC_Prescaler);
  LL_TIM_IC_SetPolarity(Config->TIMx, Config->Channel, Config->IC_Polarity);

  // Enable interrupt for the selected timer channel
  switch (Config->Channel) {
    case LL_TIM_CHANNEL_CH1: LL_TIM_EnableIT_CC1(Config->TIMx); break;
    case LL_TIM_CHANNEL_CH2: LL_TIM_EnableIT_CC2(Config->TIMx); break;
    case LL_TIM_CHANNEL_CH3: LL_TIM_EnableIT_CC3(Config->TIMx); break;
    case LL_TIM_CHANNEL_CH4: LL_TIM_EnableIT_CC4(Config->TIMx); break;
  }

  // Set timer interrupt priority and enable it in NVIC
  NVIC_SetPriority(Config->TIM_IRQn, 0);
  NVIC_EnableIRQ(Config->TIM_IRQn);

  // Enable timer counter
  LL_TIM_EnableCounter(Config->TIMx);
}

void Timer_Init(void) {
  // Enable timer clock
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);

  // Set the timer prescaler and auto-reload
  LL_TIM_SetPrescaler(TIM3, 47); // 1 kHz timer clock
  LL_TIM_SetAutoReload(TIM3, 499); // Max count


  LL_TIM_EnableIT_UPDATE(TIM3);       // Enable update interrupt
  NVIC_EnableIRQ(TIM3_IRQn);
  NVIC_SetPriority(TIM3_IRQn, 3);
  // Enable counter
  LL_TIM_EnableCounter(TIM3);
}

uint32_t micros(void) {
  uint32_t total_us = (TimerOverflows * 1000) + LL_TIM_GetCounter(TIM3);
  return total_us;
}

void TIM3_IRQHandler(void) {
  if (LL_TIM_IsActiveFlag_UPDATE(TIM3)) {
    LL_TIM_ClearFlag_UPDATE(TIM3);
    LL_GPIO_TogglePin(GPIOA, LL_GPIO_PIN_5);
    //TimerOverflows++;
  }
}
