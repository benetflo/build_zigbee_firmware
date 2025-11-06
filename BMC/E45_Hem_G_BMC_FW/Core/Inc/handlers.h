#ifndef _HANDLERS_H_
#define _HANDLERS_H_

#include <stdint.h>

void NMI_Handler(void);
void HardFault_Handler(void);
void SVC_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#endif /* _HANDLERS_H_ */
