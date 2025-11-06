//#include "SDK_i2c.h"
//#include "stm32c0xx_ll_rcc.h"
//#include "stm32c0xx_ll_i2c.h"
//#include "stm32c0xx_ll_gpio.h"
//#include "stm32c0xx_ll_bus.h"
//#include "stm32c0xx_ll_utils.h"
//
//SDK_I2C_HandleTypeDef hi2c1;
//
//#define I2C_TIMING 0x00303D5B
//
//void SDK_I2C_Init(SDK_I2C_HandleTypeDef *hi2c) {
//  LL_I2C_InitTypeDef I2C_InitStruct = {0};
//  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  LL_RCC_SetI2CClockSource(LL_RCC_I2C1_CLKSOURCE_PCLK1);
//  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);
//  /**I2C1 GPIO Configuration
//  PB8   ------> I2C1_SCL
//  PB9   ------> I2C1_SDA
//  */
//  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
//  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
//  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
//  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
//  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
//  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
//  GPIO_InitStruct.Alternate = LL_GPIO_AF_6;
//  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
//
//  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1);
//
//  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
//  I2C_InitStruct.Timing = 0x0090273D;
//  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
//  I2C_InitStruct.DigitalFilter = 2;
//  I2C_InitStruct.OwnAddress1 = 180;
//  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
//  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
//  LL_I2C_Init(I2C1, &I2C_InitStruct);
//  LL_I2C_EnableAutoEndMode(I2C1);
//  LL_I2C_SetOwnAddress2(I2C1, 0, LL_I2C_OWNADDRESS2_NOMASK);
//  LL_I2C_DisableOwnAddress2(I2C1);
//  LL_I2C_DisableGeneralCall(I2C1);
//  LL_I2C_EnableClockStretching(I2C1);
//
//  LL_I2C_Disable(I2C1);
//  LL_I2C_SetTiming(I2C1, I2C_TIMING);
//  LL_I2C_Enable(I2C1);
//}
//
//uint8_t SDK_I2C_Transmit_Polling(SDK_I2C_HandleTypeDef *hi2c, uint8_t devAddr, uint8_t *data, uint16_t size, uint32_t timeout) {
//  uint16_t index = 0;
//  uint32_t startTick = GetmsTicks();  // Get the current tick count for timeout reference
//
//  // Generate Start condition and initiate Master transmit mode
//  LL_I2C_HandleTransfer(I2C1, devAddr, LL_I2C_ADDRSLAVE_7BIT, size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);
//
//  //LL_mDelay(1);
//  // Loop until all data is transmitted or a STOP condition is detected
//  while (index < size) {
//    // Check if TXIS flag is set to ensure the transmit buffer is ready
//    if (LL_I2C_IsActiveFlag_TXIS(I2C1)) {
//      // Transmit the next byte of data
//      LL_I2C_TransmitData8(I2C1, data[index++]);
//    }
//
//    // If NACK is received, generate STOP condition and return an error
//    if (LL_I2C_IsActiveFlag_NACK(I2C1)) {
//      LL_I2C_ClearFlag_NACK(I2C1);
//      LL_I2C_GenerateStopCondition(I2C1);  // Generate STOP explicitly
//      return 1;  // Indicate error due to NACK
//    }
//  }
//
//  // Wait until the STOP flag is set (end of transmission)
//  while (!LL_I2C_IsActiveFlag_STOP(I2C1)) {
//    // Check if the timeout has been reached during STOP wait
//    if ((GetmsTicks() - startTick) > timeout) {
//      return 2;  // Indicate timeout error
//    }
//  }
//
//  // Clear the STOP flag after the transfer is complete
//  LL_I2C_ClearFlag_STOP(I2C1);
//
//  return 0;  // Transmission successful
//}
//
//uint8_t SDK_I2C_Receive_Polling(SDK_I2C_HandleTypeDef *hi2c, uint8_t devAddr, uint8_t *data, uint16_t size, uint32_t timeout) {
//  uint16_t index = 0;
//  uint32_t startTick = GetmsTicks();  // Get the current tick count for timeout reference
//
//  // Generate Start condition (master receive mode)
//  LL_I2C_HandleTransfer(I2C1, devAddr, LL_I2C_ADDRSLAVE_7BIT, size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);
//
//  // Receive data byte-by-byte
//  while (index < size) {
//    // Wait until RXNE flag (Receive Buffer Not Empty) is set, indicating data is available
//    while (!LL_I2C_IsActiveFlag_RXNE(I2C1)){
//      if ((GetmsTicks() - startTick) > timeout) {
//        LL_I2C_GenerateStopCondition(I2C1);
//        return 2;
//      }
//      // Read data from Receive Data register
//    }
//      data[index++] = LL_I2C_ReceiveData8(I2C1);
//  }
//
//  // Wait for STOP flag (indicating end of transfer)
//  while (!LL_I2C_IsActiveFlag_STOP(I2C1)){
//    if ((GetmsTicks() - startTick) > timeout) {
//      LL_I2C_GenerateStopCondition(I2C1);
//      return 2;  // Indicate timeout error
//    }
//  }
//
//  // Clear the STOP flag
//  LL_I2C_ClearFlag_STOP(I2C1);
//  return 0;
//}
//
