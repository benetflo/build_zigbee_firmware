#include <stddef.h>
#include "SDK_uart.h"
#include "ringBuf.h"

#include "stm32c0xx_ll_bus.h"
#include "stm32c0xx_ll_rcc.h"
#include "stm32c0xx_ll_gpio.h"
#include "stm32c0xx_ll_usart.h"

// Debug variables
volatile uint8_t USART1_cnt = 0;
volatile uint8_t USART2_cnt = 0;
volatile uint8_t myData1 = 0;
volatile uint8_t myData2 = 0;


rb_T UART1_RxBuf;
rb_T UART1_TxBuf;
rb_T UART2_RxBuf;
rb_T UART2_TxBuf;

void USART1_IRQHandler(void)
{
	uint8_t data;
	uint32_t ISRreg = USART1->ISR;

	USART1_cnt++;

	// Explicitly check for data input
	if(ISRreg & USART_ISR_RXNE_RXFNE){
		data  = (uint8_t)(USART1->RDR);
		myData1 = data;
		rb_writeBuf(&UART1_RxBuf, &data, 1);
	}

	// Check for framing error, overrun error or noise flag (FE=1 or ORE=1 or NF=1 in the USARTx_ISR register)

	// Check for framing error and reset
	if(ISRreg & USART_ISR_FE){
	    USART1->ICR |= USART_ICR_FECF;  // Write '1' to clear
	}

	// Check for buffer overrun and reset
	if(ISRreg & USART_ISR_ORE){
	    USART1->ICR |= USART_ICR_ORECF;  // Write '1' to clear
	}

	// Check for noise flag and reset
	if(ISRreg & USART_ISR_NE){
	    USART1->ICR |= USART_ICR_NECF;  // Write '1' to clear
	}
}

void USART2_IRQHandler(void)
{
	uint8_t data;
	uint32_t ISRreg = USART2->ISR;

	USART2_cnt++;

	// Explicitly check for data input
	if(ISRreg & USART_ISR_RXNE_RXFNE){
		data  = (uint8_t)(USART2->RDR);
		myData2 = data;
		rb_writeBuf(&UART2_RxBuf, &data, 1);
	}

	// Check for framing error, overrun error or noise flag (FE=1 or ORE=1 or NF=1 in the USARTx_ISR register)

	// Check for framing error and reset
	if(ISRreg & USART_ISR_FE){
		USART2->ICR |= USART_ICR_FECF;
	}

	// Check for buffer overrun and reset
	if(ISRreg & USART_ISR_ORE){
		USART2->ICR |= USART_ICR_ORECF;
	}

	// Check for noise flag and reset
	if(ISRreg & USART_ISR_NE){
		USART2->ICR |= USART_ICR_NECF;
	}

}

// ==================================================================================

void SEA_USART1_UART_Init(void){
  uint8_t bootString[12] = "UART1 boot\n\r";
  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  LL_RCC_SetUSARTClockSource(LL_RCC_USART1_CLKSOURCE_PCLK1);

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOB);

  /**USART1 GPIO Configuration
  USART1_TX: PB6
  USART1_RX: PB7
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_6;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_UP;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_0;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // In case no HOST is connected, a framing-error could occur triggering
  // a stream of interrupts because nothing is driving the Rx pin high.
  // Therefore, give it a pull-up.

  /* USART1 interrupt Init */
  NVIC_SetPriority(USART1_IRQn, 0);
  NVIC_EnableIRQ(USART1_IRQn);

  // Baud-rate and data formatting
  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_SetTXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_SetRXFIFOThreshold(USART1, LL_USART_FIFOTHRESHOLD_1_8);
  LL_USART_DisableFIFO(USART1);
  LL_USART_ConfigAsyncMode(USART1);

  // Enable peripheral
  LL_USART_Enable(USART1);

  /* Polling USART1 initialization */
  while((!(LL_USART_IsActiveFlag_TEACK(USART1))) || (!(LL_USART_IsActiveFlag_REACK(USART1))))
  {
  }

  /* Enable RXNE and Error interrupts */
  LL_USART_EnableIT_RXNE(USART1);
  LL_USART_EnableIT_ERROR(USART1);

  // Dummy data to check host
  LL_USART_TransmitData8(USART1, (uint8_t)'a');

  // Setup ring buffers
  rb_init(&UART1_RxBuf);
  rb_init(&UART1_TxBuf);

  rb_writeBuf(&UART1_TxBuf, &bootString[0], 12);
}


void SEA_USART2_UART_Init(void){
  LL_USART_InitTypeDef USART_InitStruct = {0};
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  /**USART2 GPIO Configuration
  PA4   ------> USART2_TX
  PA15  ------> USART2_RX
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_4;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USART2 interrupt Init */
  NVIC_SetPriority(USART2_IRQn, 0);
  NVIC_EnableIRQ(USART2_IRQn);

  /* USER CODE END USART2_Init 1 */
  USART_InitStruct.PrescalerValue = LL_USART_PRESCALER_DIV1;
  USART_InitStruct.BaudRate = 115200;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART2, &USART_InitStruct);
  LL_USART_ConfigAsyncMode(USART2);


  // Do not enable the UART2 at startup because an unpowered DUT pulls the RX line down
//  LL_USART_Enable(USART2);
//
//  /* Polling USART2 initialisation */
//  while((!(LL_USART_IsActiveFlag_TEACK(USART2))) || (!(LL_USART_IsActiveFlag_REACK(USART2))))
//  {
//  }
//
//  /* Enable RXNE and Error interrupts */
//  // Disable DUT UART interrupts at startup because it might be pulling the line down
//  LL_USART_EnableIT_RXNE(USART2);
//  LL_USART_EnableIT_ERROR(USART2);
//
//  // Dummy data to check host
//  LL_USART_TransmitData8(USART2, (uint8_t)'b');

  // Setup ring buffers
  rb_init(&UART2_RxBuf);
  rb_init(&UART2_TxBuf);
}



void SEA_UART_sendByte(USART_TypeDef *USARTx, uint8_t data){
	uint16_t retryCnt = 0;

	// Check if transmit buffer is ready to accept new data.
	while(!(USARTx->ISR & USART_CR1_TXEIE_TXFNFIE)) {
		retryCnt++;
		if(retryCnt > UART_TX_MAXRETRY){
			return;
		}
	}

	// Peripheral is ready to accept new data.
	LL_USART_TransmitData8(USARTx, data);
}
