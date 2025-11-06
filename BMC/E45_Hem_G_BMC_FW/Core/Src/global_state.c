/*
 * global_state.c
 *
 *  Created on: Jan 16, 2025
 *      Author: rogale
 */
#include "regFile.h"
#include "global_state.h"

static uint8_t global_state = 0;

void setGlobalState(uint8_t state) {
  reg_read(&global_state, REGFILE_OFFSET_DEVICE_STATE, sizeof(global_state));
  global_state |= state; // Set the bit corresponding to the state
  reg_write(REGFILE_OFFSET_DEVICE_STATE, &global_state, sizeof(global_state));
}

void resetGlobalState() {
  global_state = 0;
  reg_write(REGFILE_OFFSET_DEVICE_STATE, &global_state, sizeof(global_state));
}
uint8_t getGlobalState() {
  reg_read(&global_state, REGFILE_OFFSET_DEVICE_STATE, sizeof(global_state));
  return global_state;
}

void clearGlobalState(uint8_t state) {
  reg_read(&global_state, REGFILE_OFFSET_DEVICE_STATE, sizeof(global_state));
  global_state &= ~state; // Clear the bit corresponding to the state
  reg_write(REGFILE_OFFSET_DEVICE_STATE, &global_state, sizeof(global_state));
}

uint8_t isGlobalStateSet(uint8_t state) {
  reg_read(&global_state, REGFILE_OFFSET_DEVICE_STATE, sizeof(global_state));
  return global_state & state; // Check if the bit is set
}

uint8_t rpiState(void) {
  uint8_t rpi_state = 0;
  reg_read(&rpi_state, REGFILE_OFFSET_RPI_STATE, sizeof(rpi_state));
  return rpi_state;

}
