/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "SDK_gpio.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_exti.h"

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
        * Free pins are configured automatically as Analog (this feature is enabled through
        * the Code Generation settings)
*/
void SDK_GPIO_Init(void) {
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable necessary GPIO clocks
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOC);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOF);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOD);

  // Configure all pins to analog mode by default (for low power)
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.OutputType = LL_GPIO_MODE_INPUT;

  // Configure all pins for each port
  GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL;

  LL_GPIO_Init(GPIOC, &GPIO_InitStruct);
  LL_GPIO_Init(GPIOF, &GPIO_InitStruct);
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // Do not configure PA13 and PA14 so we can debug the board.
  GPIO_InitStruct.Pin = LL_GPIO_PIN_ALL & ~(LL_GPIO_PIN_13 | LL_GPIO_PIN_14);
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SDK_GPIO_ConfigPin(GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t output_type) {
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = pin;
  GPIO_InitStruct.Mode = mode;
  GPIO_InitStruct.Pull = pull;
  GPIO_InitStruct.Speed = speed;
  GPIO_InitStruct.OutputType = output_type;
  LL_GPIO_Init(port, &GPIO_InitStruct);
}

void SDK_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint8_t State){
  if (State == GPIO_PIN_HIGH) {
    LL_GPIO_SetOutputPin(GPIOx, Pin);
  } else {
    LL_GPIO_ResetOutputPin(GPIOx, Pin);
  }
}

uint32_t SDK_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t Pin) {
  return LL_GPIO_IsInputPinSet(GPIOx, Pin);
}

void SDK_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t Pin) {
  LL_GPIO_TogglePin(GPIOx, Pin);
}


void SDK_EXTI_ConfigPin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode, uint32_t Pull, uint32_t Trigger, IRQn_Type IRQn, uint32_t exti_source, uint32_t exti_line, uint32_t exti_config_line) {
  LL_EXTI_InitTypeDef EXTI_InitStruct = {0};

  // Set up EXTI line configuration
  EXTI_InitStruct.Line_0_31 = exti_line;
  EXTI_InitStruct.LineCommand = ENABLE;
  EXTI_InitStruct.Mode = LL_EXTI_MODE_IT;
  EXTI_InitStruct.Trigger = Trigger;
  LL_EXTI_Init(&EXTI_InitStruct);

  // Set GPIO pin mode and pull configuration
  LL_GPIO_SetPinMode(GPIOx, Pin, Mode);
  LL_GPIO_SetPinPull(GPIOx, Pin, Pull);

  // Set the EXTI source for the specified port
  LL_EXTI_SetEXTISource(exti_source, exti_config_line);

  // Configure NVIC priority and enable interrupt
  NVIC_SetPriority(IRQn, 0);
  NVIC_EnableIRQ(IRQn);
}

