#ifndef _REGFILE_H_
#define _REGFILE_H_

#include <stdint.h>

#define REG_PN                           ((uint8_t*)0x08003E00)
#define REG_SN                           ((uint8_t*)0x08003E10)
#define REG_FW                           ((uint8_t*)0x08003E20)
#define REG_HW                           ((uint8_t*)0x08003E30)

/** @def REGOFFSET_PN
 *  @brief Offset for Product Number in the register map.
 */
#define REG_OFFSET_PN                       0x00

#define REGFILE_SIZE 						142

#define REGFILE_OFFSET_PN                   0x00
#define REGFILE_OFFSET_SN                   0x01
#define REGFILE_OFFSET_FW                   0x02
#define REGFILE_OFFSET_HW                   0x03
#define REGFILE_OFFSET_MAXPAYLOAD			0x04
#define REGFILE_OFFSET_PROTOCOL				0x05

#define REGFILE_OFFSET_DEVICE_STATE			0x08
#define REGFILE_OFFSET_ERROR_STATE   		0x09
#define REGFILE_OFFSET_RPI_STATE			0x0A
#define REGFILE_OFFSET_PERIPHERAL_CONFIG    0x10

#define REGFILE_OFFSET_COLOR_LED_1			0x11
#define REGFILE_OFFSET_COLOR_LED_2			0x14
#define REGFILE_OFFSET_COLOR_LED_3			0x17
#define REGFILE_OFFSET_COLOR_LED_4			0x1A
#define REGFILE_OFFSET_COLOR_LED_5			0x1D
#define REGFILE_OFFSET_COLOR_LED_6			0x20
#define REGFILE_OFFSET_COLOR_LED_7			0x23
#define REGFILE_OFFSET_COLOR_LED_8			0x26
#define REGFILE_OFFSET_COLOR_LED_9			0x29

#define REGFILE_METRICS_VUSB_HIGH           0x40
#define REGFILE_METRICS_VUSB_LOW            0x41

#define REGFILE_METRICS_VSYS_HIGH           0x42
#define REGFILE_METRICS_VSYS_LOW            0x43
#define REGFILE_METRICS_VBAT_HIGH           0x44
#define REGFILE_METRICS_VBAT_LOW            0x45
#define REGFILE_METRICS_ISYS_HIGH           0x46
#define REGFILE_METRICS_ISYS_LOW            0x47
#define REGFILE_METRICS_TEMP_MSB            0x48
#define REGFILE_METRICS_TEMP_SECOND_BYTE    0x49
#define REGFILE_METRICS_TEMP_THIRD_BYTE     0x4a
#define REGFILE_METRICS_TEMP_LSB            0x4b

#define REGFILE_QUECTEL_CONFIG				0x50

#define REGFILE_STATE_ERROR_FLAG		    (1 << 0)
#define REGFILE_STATE_BATT_PWR_ON      		(1 << 1)
#define REGFILE_STATE_CHARGE_EN        		(1 << 2)
#define REGFILE_STATE_MAINTENANCE			(1 << 3)
#define REGFILE_STATE_STANDBY				(1 << 4)

#define REGFILE_CONFIG_SIREN_EN				(1 << 0)
#define REGFILE_CONFIG_LED_USER_CTRL		(1 << 1)

#define REGFILE_QUECTEL_AP_READY            (1 << 0)
#define REGFILE_QUECTEL_USB_BOOT            (1 << 1)
#define REGFILE_QUECTEL_PSM_EINT            (1 << 2)
#define REGFILE_QUECTEL_RESET_n             (1 << 3)

// ====================================================================
// FUNCTIONS
void reg_init(void);
void reg_trackChanges(void);
void reg_read(uint8_t* dest, uint8_t address, uint8_t length);
uint8_t reg_write(uint8_t address, uint8_t* src, uint8_t length);

void reg_CONFIG_handler(void);
void reg_DEVICE_STATE_handler(void);
void reg_OpMetrics_handler(void);

#endif /* INC_REGFILE_H_ */
