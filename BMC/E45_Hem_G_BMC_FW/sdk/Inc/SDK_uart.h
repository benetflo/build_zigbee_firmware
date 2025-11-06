/*
 * SEA_uart.h
 *
 *  Created on: Nov 13, 2024
 *      Author: jurber
 */

#ifndef INC_SDK_UART_H_
#define INC_SDK_UART_H_

	#include "stm32c0xx_ll_usart.h"
	#include "ringBuf.h"

	#define UART_TX_MAXRETRY		1000

	extern rb_T UART1_RxBuf;
	extern rb_T UART1_TxBuf;
	extern rb_T UART2_RxBuf;
	extern rb_T UART2_TxBuf;

	void SEA_USART1_UART_Init(void);
	void SEA_USART2_UART_Init(void);

	void SEA_UART_sendByte(USART_TypeDef *USARTx, uint8_t data);


#endif /* INC_SDK_UART_H_ */
