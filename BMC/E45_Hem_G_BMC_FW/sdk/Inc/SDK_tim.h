#ifndef _SDK_TIM_H_
#define _SDK_TIM_H_

#include "stm32c0xx_ll_tim.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_bus.h"

typedef struct {
    TIM_TypeDef *TIMx;       // Timer instance (e.g., TIM1, TIM2, etc.)
    uint32_t Channel;        // Timer channel (e.g., LL_TIM_CHANNEL_CH1)
    GPIO_TypeDef *GPIOx;     // GPIO port (e.g., GPIOA, GPIOB)
    uint32_t GPIO_Pin;       // GPIO pin (e.g., LL_GPIO_PIN_6)
    uint32_t GPIO_AF;        // GPIO alternate function (e.g., LL_GPIO_AF_2)
    uint32_t IC_Polarity;    // Input capture polarity (e.g., LL_TIM_IC_POLARITY_RISING)
    uint32_t IC_Prescaler;   // Input capture prescaler (e.g., LL_TIM_ICPSC_DIV1)
    IRQn_Type TIM_IRQn;      // Timer IRQ number (e.g., TIM1_CC_IRQn)
} SDK_TIM_InitTypeDef;

// Function prototypes
void SDK_TIM_InputCapture_Init(SDK_TIM_InitTypeDef *Config);
void Timer_Init(void);
uint32_t micros(void);

#endif /* _SDK_TIM_H_ */
