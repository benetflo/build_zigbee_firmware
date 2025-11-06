/**
  * @brief System Clock Configuration
  * @retval None
  */

#include "system_init.h"
#include "stm32c0xx_ll_system.h"
#include "stm32c0xx_ll_rcc.h"
#include "stm32c0xx_ll_utils.h"
#include "stm32c0xx_ll_bus.h"

// Configures system clocks and power
void InitializeSystem(void) {
  // Enable SYSCFG and PWR clocks
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  // Configure system clock
  SystemClock_Config1();
  // Configure system tick
  SysTick_Config(48000); // 48000000Hz x 0.001s to get a tick every 1ms.
}

void SystemClock_Config1(void) {
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* HSI configuration and activation */
  LL_RCC_HSI_Enable();
  while(LL_RCC_HSI_IsReady() != 1){}

  LL_RCC_HSI_SetCalibTrimming(64);
  LL_RCC_SetHSIDiv(LL_RCC_HSI_DIV_1);
  /* Set AHB prescaler*/
  LL_RCC_SetAHBPrescaler(LL_RCC_HCLK_DIV_1);

  /* Sysclk activation on the HSI */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSI){}

  /* Set APB1 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(48000000);
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(48000000);
}


