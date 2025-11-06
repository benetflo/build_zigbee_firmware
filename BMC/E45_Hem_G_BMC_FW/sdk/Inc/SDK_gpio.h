#ifndef _SDK_GPIO_H_
#define _SDK_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "stm32c031xx.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_exti.h"


#define GPIO_PIN_HIGH    (1U)  // Pin state is high
#define GPIO_PIN_LOW     (0U)  // Pin state is low

void SDK_GPIO_Init(void);
void SDK_GPIO_ConfigPin(GPIO_TypeDef *port, uint32_t pin, uint32_t mode, uint32_t pull, uint32_t speed, uint32_t output_type);
void SDK_GPIO_WritePin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint8_t State);
uint32_t SDK_GPIO_ReadPin(GPIO_TypeDef *GPIOx, uint32_t Pin);
void SDK_GPIO_TogglePin(GPIO_TypeDef *GPIOx, uint32_t Pin);
void SDK_EXTI_ConfigPin(GPIO_TypeDef *GPIOx, uint32_t Pin, uint32_t Mode, uint32_t Pull, uint32_t Trigger, IRQn_Type IRQn, uint32_t exti_source, uint32_t exti_line, uint32_t exti_config_line);
/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*_SDK_GPIO_H_ */

