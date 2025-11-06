/*
 * ringBuf.c
 *
 *  Created on: Nov 14, 2024
 *      Author: jurber
 */

#include "ringBuf.h"


// This function increments the readPtr while taking overflow into account.
static void rb_incBufReadPtr(rb_T* buf) {
	// Check for overflow, else increment linearly
	if(buf->readPtr == buf->bufEnd){
		buf->readPtr = (uint8_t*) buf->bufStart;
	}
	else{
		buf->readPtr++;
	}
}


// This function increments the writePtr while taking overflow into account.
static void rb_incBufWritePtr(rb_T* buf) {
	// Check for overflow, else increment linearly
	if(buf->writePtr == buf->bufEnd){
		buf->writePtr = (uint8_t*) buf->bufStart;
	}
	else{
		buf->writePtr++;
	}
}

// =========================================================================================
// Externally accessible

// Init the ring buffer
void rb_init(rb_T* rb) {
    rb->readPtr = &rb->buf[0];
    rb->writePtr = &rb->buf[0];
    rb->bufStart = &rb->buf[0];
    rb->bufEnd = &rb->buf[RINGBUF_BUFSIZE - 1];
}

// This function writes to the buffer, without checking for buffer overrun.
void rb_writeBuf(rb_T* buf, uint8_t* srcArray, uint8_t length){
	uint16_t n;

	// Overflow condition
	if(length > RINGBUF_BUFSIZE){
		length = RINGBUF_BUFSIZE;
	}

	for(n=0 ; n<length ; n++){
		rb_incBufWritePtr(buf);
		*(buf->writePtr) = *(srcArray+n);
	}
}

/* 	This function reads data from a buffer until it hits either
	a maximum readLength or there is no more new data in the buffer.
 */
uint8_t rb_readBuf(rb_T* buf, uint8_t* destArray, uint8_t length){
	uint8_t n;

	// Overflow condition
	if(length > RINGBUF_BUFSIZE){
		length = RINGBUF_BUFSIZE;
	}

	// Check if there's any data at all.
	if(buf->readPtr == buf->writePtr){
		return 0;
	}

	// Apparently there's data; start extracting it.
	for(n=0; n<length; n++){
		rb_incBufReadPtr(buf);
		*(destArray+n) = *(buf->readPtr);

		// Check if there's data
		if(buf->readPtr == buf->writePtr){
			break;
		}
	}

	return n+1;
}

// This function returns a bool whether there is unread data
uint8_t rb_checkUnhandledData(rb_T* buf) {
	if(buf->readPtr == buf->writePtr) {
		return 0;
	} else {
		return 1;
	}

}

