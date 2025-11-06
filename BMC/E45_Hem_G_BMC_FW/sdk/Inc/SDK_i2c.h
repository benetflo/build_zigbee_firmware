//#ifndef _SDK_I2C_H_
//#define _SDK_I2C_H_
//
//#include <stdint.h>
//#include "SDK_systick.h"
//
//// I2C State Machine
//typedef enum {
//    I2C_READY,
//    I2C_BUSY,
//    I2C_ERROR
//} SDK_I2C_State;
//
//typedef struct {
//    volatile SDK_I2C_State state;
//    uint8_t *txBuffer;
//    uint8_t *rxBuffer;
//    uint16_t txSize;
//    uint16_t rxSize;
//    uint16_t txIndex;
//    uint16_t rxIndex;
//} SDK_I2C_HandleTypeDef;
//
//extern SDK_I2C_HandleTypeDef hi2c1;
//
//void SDK_I2C_Init(SDK_I2C_HandleTypeDef *hi2c);
//uint8_t SDK_I2C_Transmit_Polling(SDK_I2C_HandleTypeDef *hi2c, uint8_t devAddr, uint8_t *data, uint16_t size, uint32_t timeout);
//uint8_t SDK_I2C_Receive_Polling(SDK_I2C_HandleTypeDef *hi2c, uint8_t devAddr, uint8_t *data, uint16_t size, uint32_t timeout);
//
//#endif /* _SDK_I2C_H_ */
