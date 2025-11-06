#ifndef _COMMS_H_
#define _COMMS_H_

#include <stdint.h>

#define COMMS_RX_MAXLENGTH		32				// How many bytes may be parsed within one loop interrupt.
#define COMMS_TX_MAXLENGTH		128				// How many bytes may be sent within one loop interrupt.
#define COMMS_PACKET_MAXLENGTH	128				// Length in bytes how long a packet may be.

// Query command codes
#define COMMS_CMD_Q_REGREAD		0x1				// Register read command.
#define COMMS_CMD_Q_REGWRITE	0x2				// Register write command.
#define COMMS_CMD_Q_RESET		0x3				// System reset command.
#define COMMS_CMD_Q_FWDPAYLOAD	0x71			// Forward the payload downstream without stuffing.

// Reply command codes
#define COMMS_CMD_R_REGREAD		0x81
#define COMMS_CMD_R_REGWRITE	0x82
#define COMMS_CMD_R_RESET		0x83			// This command is never sent, instead the system resets immediately.
#define COMMS_CMD_R_FWDPAYLOAD	0xF1

#define DLE						0xFC
#define STX						0xFD
#define ETX						0xFE

// ====================================================================
// Data types and declarations
typedef struct comms_packet_S{
  uint8_t payload[COMMS_PACKET_MAXLENGTH];	// Array to hold the payload.
  uint8_t cnt;								// Counter how many bytes of payload have been received.
  uint8_t chkSum;								// Checksum calculated over the payload section.
  uint8_t valid;								// Flag to indicate whether a valid packet has been extracted.
} comms_packet_T;

typedef enum comms_extractorState_N {
  comms_extractorState_Idle = 0,
  comms_extractorState_DLE,
  comms_extractorState_Payload,
} comms_extractorState_T;

typedef struct comms_extractor_S {
  comms_extractorState_T state;				// Current state of the extractor
  comms_extractorState_T nextState;			// Next state of the extractor
  comms_packet_T pkt;							// Packet container
} comms_extractor_T;

// ====================================================================
// FUNCTIONS
void comms_init(void);
void comms_handler(void);
void comms_receiveHandler(void);
void comms_transmitHandler(void);

void comms_extractPacket(comms_extractor_T* xt, uint8_t* data);
void comms_packetParser(comms_packet_T* pkt);
uint8_t comms_calcChkSum(comms_packet_T* pkt);

#endif /* _COMMS_H_ */
