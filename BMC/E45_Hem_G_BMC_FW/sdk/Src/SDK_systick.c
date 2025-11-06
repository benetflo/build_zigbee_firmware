#include <stdint.h>
#include "SDK_systick.h"

static volatile uint32_t msTicks = 0;

uint32_t SDK_GetTick_Diff(uint32_t current, uint32_t last) {
  // Handle overflow case
  if (current < last) {
    return (UINT32_MAX - last) + current + 1;
  }
  return current - last;
}

void SysTick_Handler(void) {
  msTicks++;
}

uint32_t GetmsTicks(void) {
  return msTicks;
}
