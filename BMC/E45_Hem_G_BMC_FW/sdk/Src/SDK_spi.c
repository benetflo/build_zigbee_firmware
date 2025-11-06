/*
 * SDK_spi.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Dell
 */

#include <stddef.h>
#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_gpio.h"
#include "SDK_spi.h"

static volatile uint8_t *txBuffer = NULL;
static volatile uint16_t txSize = 0;
static volatile uint16_t txIndex = 0;


// Initialize SPI
void SDK_SPI_Init(void) {
  // Enable clocks for SPI1 and GPIOA
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SPI1);

  // Configure GPIO for SPI (PA1: SCK, PA2: MOSI)
  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_1, LL_GPIO_AF_0);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_1, LL_GPIO_SPEED_FREQ_LOW);

  LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
  LL_GPIO_SetAFPin_0_7(GPIOA, LL_GPIO_PIN_2, LL_GPIO_AF_0);
  LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_LOW);

  // Configure SPI
  LL_SPI_SetMode(SPI1, LL_SPI_MODE_MASTER);
  LL_SPI_SetDataWidth(SPI1, LL_SPI_DATAWIDTH_8BIT);
  LL_SPI_SetTransferDirection(SPI1, LL_SPI_FULL_DUPLEX);
  LL_SPI_SetClockPolarity(SPI1, LL_SPI_POLARITY_LOW);
  LL_SPI_SetClockPhase(SPI1, LL_SPI_PHASE_1EDGE);
  LL_SPI_SetBaudRatePrescaler(SPI1, LL_SPI_BAUDRATEPRESCALER_DIV32);
  LL_SPI_SetTransferBitOrder(SPI1, LL_SPI_MSB_FIRST);
  LL_SPI_SetNSSMode(SPI1, LL_SPI_NSS_SOFT);

  // Enable SPI
  LL_SPI_Enable(SPI1);

  // Enable SPI interrupt
  NVIC_SetPriority(SPI1_IRQn, 0);
  NVIC_EnableIRQ(SPI1_IRQn);
}

// Transmit data using interrupt
void SDK_SPI_Transmit_IT(uint8_t *data, uint16_t size) {
  txBuffer = data;
  txSize = size;
  txIndex = 0;

  // Enable TXE interrupt
  LL_SPI_EnableIT_TXE(SPI1);
}

// SPI Interrupt Handler
void SPI1_IRQHandler(void) {
  // Handle transmit interrupt
  if (LL_SPI_IsActiveFlag_TXE(SPI1) && txIndex < txSize) {
    LL_SPI_TransmitData8(SPI1, txBuffer[txIndex++]);
  }

  // Check if transmission is complete
  if (!LL_SPI_IsActiveFlag_BSY(SPI1)) {
    // Disable SPI interrupts
    LL_SPI_DisableIT_TXE(SPI1);
  }
}
