/*
 * ringBuf.h
 *
 *  Created on: Nov 14, 2024
 *      Author: jurber
 */

#ifndef INC_RINGBUF_H_
#define INC_RINGBUF_H_

	#include <stdint.h>

	#define RINGBUF_BUFSIZE 		255


	// ====================================================================
	// Data types and declarations
	typedef struct rb_S{
		uint8_t* readPtr;
		uint8_t* writePtr;
		uint8_t buf[RINGBUF_BUFSIZE];
		const uint8_t* bufStart;
		const uint8_t* bufEnd;
	}rb_T;


	// ====================================================================
	// FUNCTIONS
	void rb_init(rb_T* rb);


//	static void rb_incBufReadPtr(rb_T* buf);
//	static void rb_incBufWritePtr(rb_T* buf);
	void rb_writeBuf(rb_T* buf, uint8_t* srcArray, uint8_t length);
	uint8_t rb_readBuf(rb_T* buf, uint8_t* destArray, uint8_t length);
	uint8_t rb_checkUnhandledData(rb_T* buf);

#endif /* INC_RINGBUF_H_ */
