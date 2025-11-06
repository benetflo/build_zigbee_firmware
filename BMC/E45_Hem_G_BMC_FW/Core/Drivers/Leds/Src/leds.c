/*
 * leds.c
 *
 *  Created on: Nov 28, 2024
 *      Author: Dell
 */
#include <string.h>
#include "SDK_spi.h"
#include "leds.h"

static uint8_t colorArray[4*LED_COUNT + 8];

uint32_t GenerateColorCommand(uint8_t flag, uint8_t dimming, uint8_t red, uint8_t green, uint8_t blue) {
  uint32_t color = 0;
  color |= (flag & 0x07) << 29;
  color |= (dimming & 0x1f) << 24;
  color |= (blue & 0xff) << 16;
  color |= (green & 0xff) << 8;
  color |= (red & 0xff);

  return __REV(color);
}

void SendColorCommandArray(uint8_t *data, uint16_t size){
  uint8_t startFrame[] = {0x00, 0x00, 0x00, 0x00};
  uint8_t endFrame[] = {0xff, 0xff, 0xff, 0xff};

  memcpy(colorArray, startFrame, sizeof(startFrame));
  memcpy(colorArray + sizeof(startFrame), data, size);
  memcpy(colorArray + sizeof(startFrame) + size, endFrame, sizeof(endFrame));

  // Transmit the entire message
  SDK_SPI_Transmit_IT(colorArray, sizeof(colorArray));
}
