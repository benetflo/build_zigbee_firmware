/*
 * ledFsm.c
 *
 *  Created on: Nov 30, 2024
 *      Author: Dell
 */
#include <led_fsm.h>
#include "main.h"
#include "leds.h"
#include <stdint.h>
#include "stm32c0xx_ll_utils.h"
#include "SDK_systick.h"
#include "regFile.h"
#include "global_state.h"

// Enum for LED colors/modes
typedef enum {
  LED_COLOR_RGB_SWIRL,
  LED_COLOR_GREEN,
  LED_COLOR_RED_FLASH,
  LED_COLOR_BLUE_PULSE,
  LED_COLOR_YELLOW_BLINK,
  LED_COLOR_USER_DEFINED,
  LED_COLOR_UNDEFINED
} LedColor;

void LedSwirl() {
  static uint32_t colors[LED_COUNT];
  static uint8_t red = 255;
  static uint8_t green = 0;
  static uint8_t blue = 0;
  static uint8_t direction = 1;
  const uint8_t stepSize = 5;

  if (direction == 0) {
    red = (red + stepSize > 255) ? 255 : red + stepSize;
    green = (green < stepSize) ? 0 : green - stepSize;
    if (red == 255 && green == 0) {
      direction = 1;
    }
  } else if (direction == 1) {
    green = (green + stepSize > 255) ? 255 : green + stepSize;
    blue = (blue < stepSize) ? 0 : blue - stepSize;
    if (green == 255 && blue == 0) {
      direction = 2;
    }
  } else {
    blue = (blue + stepSize > 255) ? 255 : blue + stepSize;
    red = (red < stepSize) ? 0 : red - stepSize;
    if (blue == 255 && red == 0) {
      direction = 0;
    }
  }

  // Update LED colors
  for (uint8_t i = 1; i < LED_COUNT; i++) {
    colors[i] = colors[i - 1];
  }
  //colors[0] = GenerateColorCommand(0b101, 0x00, red, green, blue);
  colors[0] = GenerateColorCommand(0b111, 0x1F, red, green, blue);

  // Send the color command array to the LEDs
  SendColorCommandArray((uint8_t *)colors, sizeof(colors));
}


void LedsToGreen(void) {
  uint32_t colors[LED_COUNT];  // Array to hold the color commands

  // Generate green color for each LED
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    colors[i] = GenerateColorCommand(0b111, 0x1f, 0x00, 0x01, 0x00);  // Green: Red=0, Green=255, Blue=0
  }

  // Send the green color to all LEDs
  SendColorCommandArray((uint8_t *)colors, sizeof(colors));
}

void LedsToPulsingBlue(void) {
  uint32_t pulse_interval_ms = 10;  // Update the brightness every 10ms
  static uint32_t last_pulse_time = 0;
  uint32_t current_time = GetmsTicks();  // Get the current system time in milliseconds

  // Check if the pulse interval has passed
  if (current_time - last_pulse_time >= pulse_interval_ms) {
    static int8_t brightness_step = 5;  // Change in brightness per update
    static uint8_t blue_brightness = 0;  // Current brightness of the blue channel
    uint32_t colors[LED_COUNT];         // Array to hold the color commands

    // Update brightness
    blue_brightness += brightness_step;

    // Reverse direction at limits
    if (blue_brightness == 255 || blue_brightness == 0) {
      brightness_step = -brightness_step;  // Reverse the step direction
    }

    // Set all LEDs to the current blue brightness
    for (uint8_t i = 0; i < LED_COUNT; i++) {
      colors[i] = GenerateColorCommand(0b111, 0x1f, 0x00, 0x00, blue_brightness);
    }

    // Send the color command to all LEDs
    SendColorCommandArray((uint8_t *)colors, sizeof(colors));

    last_pulse_time = current_time;  // Update the last pulse time
  }
}

void LedsToBlinkingYellow(void) {
  uint32_t blink_interval_ms = 500;  // Blink every 500ms
  static uint32_t last_blink_time = 0;
  uint32_t current_time = GetmsTicks();  // Get the current system time in milliseconds

  // Check if the blink interval has passed
  if (current_time - last_blink_time >= blink_interval_ms) {
    static uint8_t leds_on = 0;         // Tracks whether LEDs are on or off
    uint32_t colors[LED_COUNT];        // Array to hold the color commands

    if (leds_on) {
      // Turn LEDs off
      for (uint8_t i = 0; i < LED_COUNT; i++) {
        colors[i] = GenerateColorCommand(0b111, 0x1f, 0x00, 0x00, 0x00);  // Off: Red=0, Green=0, Blue=0
      }
    } else {
      // Set LEDs to yellow (Red + Green)
      for (uint8_t i = 0; i < LED_COUNT; i++) {
        colors[i] = GenerateColorCommand(0b111, 0x1f, 0xff, 0xff, 0x00);  // Yellow: Red=255, Green=255, Blue=0
      }
    }

    // Send the color command to all LEDs
    SendColorCommandArray((uint8_t *)colors, sizeof(colors));

    leds_on = !leds_on;  // Toggle the LED state
    last_blink_time = current_time;  // Update the last blink time
  }
}


void LedsToFlashingRed(void){
  uint32_t flash_interval_ms = 100;
  static uint32_t last_flash_time = 0;
  uint32_t current_time = GetmsTicks();  // Get the current system time in milliseconds

  // Check if the flash interval has passed
  if (current_time - last_flash_time >= flash_interval_ms) {
    static uint8_t leds_on = 0;
    uint32_t colors[LED_COUNT];  // Array to hold the color commands
    if (leds_on) {
      // Turn LEDs off
      for (uint8_t i = 0; i < LED_COUNT; i++) {
        colors[i] = GenerateColorCommand(0b111, 0x1f, 0x00, 0x00, 0x00);  // Off: Red=0, Green=0, Blue=0
      }
    } else {
      // Turn LEDs to red
      for (uint8_t i = 0; i < LED_COUNT; i++) {
        colors[i] = GenerateColorCommand(0b111, 0x1f, 0xff, 0x00, 0x00);  // Red: Red=255, Green=0, Blue=0
      }
    }

    // Send the color command to all LEDs
    SendColorCommandArray((uint8_t *)colors, sizeof(colors));

    leds_on = !leds_on;  // Toggle the LED state
    last_flash_time = current_time;  // Update the last flash time
  }
}

void LedsToUserDefined(void) {
  uint8_t regColors[REG_COLOR_LEDS];
  uint8_t regColorsIndex = 0;
  uint32_t colors[LED_COUNT];
  reg_read(regColors, 0x11, sizeof(regColors));
  for (uint8_t i = 0; i < LED_COUNT; i++) {
    colors[i] = GenerateColorCommand(0b111, 0x1f, regColors[regColorsIndex], regColors[regColorsIndex+1], regColors[regColorsIndex + 2]);
    regColorsIndex += 3;
  }

  SendColorCommandArray((uint8_t *)colors, sizeof(colors));
}

LedColor getColorForActualState(void) {
  LedColor color = LED_COLOR_UNDEFINED;
  uint8_t configReg = 0;
  reg_read(&configReg, 0x10, 1);

  if (configReg & REGFILE_CONFIG_LED_USER_CTRL) {
    return LED_COLOR_USER_DEFINED;
  }

  uint8_t state = getGlobalState();

  if (!rpiState()) {
	  state = INIT_STATE;
  }

  if (state & ERR_FLAG_STATE) {
    color = LED_COLOR_RED_FLASH;
  } else if (state & INIT_STATE) {
    color = LED_COLOR_RGB_SWIRL;
  } else if (state & STANDBY_STATE) {
    color = LED_COLOR_BLUE_PULSE;
  } else if (state & MAINTENANCE_STATE) {
    color = LED_COLOR_YELLOW_BLINK;
  } else if (state & SHUTDOWN_STATE){
	color = LED_COLOR_BLUE_PULSE;
  } else {
	color = LED_COLOR_GREEN;
  }
  return color;
}

// Function to update LED state
void updateLedState() {
  // State machine with switch case
  LedColor color = getColorForActualState();

  switch(color) {
    case LED_COLOR_RGB_SWIRL:
      LedSwirl();
      break;
    case LED_COLOR_GREEN:
      LedsToGreen();
      break;
    case LED_COLOR_RED_FLASH:
	  LedsToFlashingRed();
      break;
    case LED_COLOR_BLUE_PULSE:
      LedsToPulsingBlue();
      break;
    case LED_COLOR_YELLOW_BLINK:
      LedsToBlinkingYellow();
      break;
    case LED_COLOR_USER_DEFINED:
      LedsToUserDefined();
      break;
    default:
	  LedsToGreen();
	  break;
  }
}
