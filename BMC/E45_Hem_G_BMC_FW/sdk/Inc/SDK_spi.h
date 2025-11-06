/*
 * SDK_spi.h
 *
 *  Created on: Nov 28, 2024
 *      Author: Dell
 */

#ifndef _SDK_SPI_H_
#define _SDK_SPI_H_

#include "stm32c0xx_ll_spi.h"

void SDK_SPI_Init(void);
void SDK_SPI_Transmit_IT(uint8_t *data, uint16_t size);


#endif /* _SDK_SPI_H_ */
