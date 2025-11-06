/*
 * SDK_systick.h
 *
 *  Created on: Nov 9, 2024
 *      Author: Dell
 */

#ifndef _SDK_SYSTICK_H_
#define _SDK_SYSTICK_H_

#include <stdint.h>
#include "cmsis_compiler.h"

void SysTick_Handler(void);
uint32_t SDK_GetTick_Diff(uint32_t current, uint32_t last);
uint32_t GetmsTicks(void);

#endif /* _SDK_SYSTICK_H_ */
