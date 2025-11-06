/*
 * comms.c
 *
 *  Created on: Nov 14, 2024
 *      Author: jurber
 */

#include <SDK_uart.h>
#include "comms.h"
#include "regFile.h"
#include "ringBuf.h"

// TODO: add keep-alive packet / watchdog reset on comms

comms_extractor_T xt_Rx;

void comms_init(void) {
	// Set the startup state for the packet extractor. Note only one needed (for USART1: HOST).
	xt_Rx.state = comms_extractorState_Idle;
	xt_Rx.nextState = comms_extractorState_Idle;
	xt_Rx.pkt.valid = 0;
	xt_Rx.pkt.chkSum = 0;
	xt_Rx.pkt.cnt = 0;
}

void comms_handler(void) {
	comms_receiveHandler();
	comms_packetParser(&xt_Rx.pkt);
	comms_transmitHandler();
}

// This function checks if there's unhandled data in the receive buffers.
void comms_receiveHandler(void) {
	uint8_t n, data;


	// Check for queued incoming data on UART1 from Host
	for(n=0 ; n<COMMS_RX_MAXLENGTH ; n++) {
		if(rb_checkUnhandledData(&UART1_RxBuf)) {
			rb_readBuf(&UART1_RxBuf, &data, 1);

			// Try and extract as part of a packet
			comms_extractPacket(&xt_Rx, &data);

			// If there is a valid packet, break to prevent overrun
			if(xt_Rx.pkt.valid){
				break;
			}
		} else {
			break;
		}
	}

	// Check incoming data on UART2 from DUT and forward on UART1 to HOST
	if(rb_checkUnhandledData(&UART2_RxBuf)) {
		// Forward on UART1 to HOST
		rb_readBuf(&UART2_RxBuf, &data, 1);
		rb_writeBuf(&UART1_TxBuf, &data, 1);
	}
}

void comms_transmitHandler(void) {
	uint8_t n, data;

	// Check for queued outgoing data on UART1 to Host
	for(n=0 ; n<COMMS_TX_MAXLENGTH ; n++) {
		if(rb_checkUnhandledData(&UART1_TxBuf)) {
			rb_readBuf(&UART1_TxBuf, &data, 1);
			SEA_UART_sendByte(USART1, data);
		} else {
			break;
		}
	}

	// Check for queued outgoing data on UART2 to DUT
	for(n=0 ; n<COMMS_TX_MAXLENGTH ; n++) {
		if(rb_checkUnhandledData(&UART2_TxBuf)) {
			rb_readBuf(&UART2_TxBuf, &data, 1);
			SEA_UART_sendByte(USART2, data);
		} else {
			break;
		}
	}
}

// This function tries to unstuff and extract a packet from a byte-stream.
// This function should be called on a per-byte basis from an ordered stream of bytes.
// Note that only the HOST send a packet, the DUT sends unstructured strings of various lengths.
void comms_extractPacket(comms_extractor_T* xt, uint8_t* data) {
	uint8_t localChkSum = 0;
	uint8_t n;

	switch(xt->state) {
	case comms_extractorState_Idle:
		if(*data == DLE) {
			xt->nextState = comms_extractorState_DLE;
		}
		break;

	case comms_extractorState_DLE:
		if(*data == DLE) {
			// Previous character was stuffed, store as if payload
			xt->pkt.payload[xt->pkt.cnt] = *data;
			xt->pkt.cnt++;

			xt->nextState = comms_extractorState_Payload;
		} else if(*data == STX) {
			// Start of new packet, reset everything.
			xt->pkt.valid = 0;
			xt->pkt.cnt = 0;
			xt->pkt.chkSum = 0;

			xt->nextState = comms_extractorState_Payload;
		} else if(*data == ETX) {
			// We've reached the end of the packet.
			// Last entry should have been the checksum, so deduct.
			xt->pkt.chkSum = xt->pkt.payload[xt->pkt.cnt-1];
			xt->pkt.cnt--;

			// Validate its checksum.
			localChkSum = 0;
			for(n=0 ; n<(xt->pkt.cnt) ; n++){
				localChkSum += xt->pkt.payload[n];
			}

			// If checksum if valid, mark packet as valid.
			if(localChkSum == comms_calcChkSum(&xt->pkt)) {
				xt->pkt.valid = 1;
			}

			// Regardless of valid packet, return to idle
			xt->nextState = comms_extractorState_Idle;
		}
		break;

	case comms_extractorState_Payload:
		if(*data == DLE) {		// Find out if it's due to stuffing or upcoming escape character
			xt->nextState = comms_extractorState_DLE;
		} else {
			// Add data to payload section, and update checksum
			xt->pkt.payload[xt->pkt.cnt] = *data;
			xt->pkt.cnt++;
		}
		break;
	}

	xt->state = xt->nextState;
}

// This function performs byte-stuffing on a packet and pushes it onto a ringbuffer.
void comms_stuffPacket(comms_packet_T* pkt, rb_T* buf){
	uint8_t n;
	uint8_t delimiter = DLE;
	uint8_t startToken = STX;
	uint8_t stopToken = ETX;

	// Write header to buffer.
	rb_writeBuf(buf, &delimiter, 1);		// DLE
	rb_writeBuf(buf, &startToken, 1);		// STX

	// Destuff packet and write to buffer.
	for(n=0 ; n < pkt->cnt ; n++){
		if(pkt->payload[0] == DLE){
			rb_writeBuf(buf, &delimiter, 1);		// Stuff with DLE
		}
		rb_writeBuf(buf, &pkt->payload[n], 1);		// CMD
	}

	// Write checksum
	if(pkt->chkSum == DLE){
		rb_writeBuf(buf, &delimiter, 1);	// Stuff with DLE
	}
	rb_writeBuf(buf, &pkt->chkSum, 1);		// CMD

	// Write footer to buffer.
	rb_writeBuf(buf, &delimiter, 1);		// DLE
	rb_writeBuf(buf, &stopToken, 1);		// ETX
}


// This function calculates the checksum of an existing packet.
uint8_t comms_calcChkSum(comms_packet_T* pkt){
	uint8_t localChkSum = 0;
	uint8_t n;

	for(n=0 ; n < pkt->cnt ; n++) {
		localChkSum += pkt->payload[n];
	}

	return localChkSum;
}

// ====================================================================
// Packet parsing


// Packet is defined in the following format:
// Byte#	| Description
// -------------------------------------------
// 0		| Command
// [1..]	| Unique payload per command
//
void comms_packetParser(comms_packet_T* pkt) {
	comms_packet_T pktReply = {.cnt=0, .payload={0}, .chkSum=0, .valid=0};

	// First check if there is a valid packet
	if(pkt->valid){
		// Look at the first byte in the payload to determine command
		switch(pkt->payload[0]){
			/* COMMS_CMD_Q_REGREAD
			Byte#	| Description
			-------------------------------------------
			0		| Command
			1		| Address
			2		| Read length
			*/
			case COMMS_CMD_Q_REGREAD:
				// Perform regRead directly into payload of reply packet
				reg_read(&pktReply.payload[3], pkt->payload[1], pkt->payload[2]);

				// Construct reply to HOST on UART1
				pktReply.payload[0] = COMMS_CMD_R_REGREAD;				// command
				pktReply.payload[1] = pkt->payload[1];					// address at which the read started
				pktReply.payload[2] = pkt->payload[2];					// length which was issued by Host
				pktReply.cnt = 3 + pkt->payload[2];						// packet length counter
				pktReply.chkSum = comms_calcChkSum(&pktReply);			// checksum
				pktReply.valid = 1;										// not used
				comms_stuffPacket(&pktReply, &UART1_TxBuf);
			break;

			/* COMMS_CMD_Q_REGWRITE
			Byte#	| Description
			-------------------------------------------
			0		| Command
			1		| Address
			[2..]	| Data
			*/
			case COMMS_CMD_Q_REGWRITE:
				// Construct reply to HOST on UART1
				pktReply.payload[0] = COMMS_CMD_R_REGWRITE;				// command
				pktReply.payload[1] = pkt->payload[1];					// address at which the write started
				pktReply.payload[2] = pkt->cnt - 2;						// length written by regHandler
				pktReply.cnt = 3;										// packet length counter
				pktReply.chkSum = comms_calcChkSum(&pktReply);			// checksum
				pktReply.valid = 1;										// not used
				comms_stuffPacket(&pktReply, &UART1_TxBuf);

				// Perform regWrite last so comms buffers stay clean, store actual length written
				// Write length is pkt->cnt, minus CMD, minus ADDRESS
				reg_write(pkt->payload[1], &pkt->payload[2], pkt->cnt - 2);
			break;

			/* COMMS_CMD_Q_RESET
			Byte#	| Description
			-------------------------------------------
			0		| Command
			*/
			case COMMS_CMD_Q_RESET:
				NVIC_SystemReset();
			break;

			/* COMMS_CMD_Q_FWDPAYLOAD
			Byte#	| Description
			-------------------------------------------
			0		| Command
			[1..]	| Data
			*/
			case COMMS_CMD_Q_FWDPAYLOAD:
				// Take payload and forward to DUT on UART2
				rb_writeBuf(&UART2_TxBuf, &pkt->payload[1], pkt->cnt -1);		// DLE

				// No reply to HOST on UART1
				asm("NOP");
			break;
		}
	}

	// Reset valid flag to signal handle completion
	pkt->valid = 0;
}
