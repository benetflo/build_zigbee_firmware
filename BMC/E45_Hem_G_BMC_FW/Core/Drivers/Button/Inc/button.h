#ifndef _Button_H_
#define _Button_H_

#include <stdint.h>

void EXTI4_15_IRQHandler(void);
void clearButtonState(void);
uint8_t isButtonReleased(void);
uint8_t isButtonPressed(void);
void keepButtonReleased(void);

#endif /* _HANDLERS_H_ */
