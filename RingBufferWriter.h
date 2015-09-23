/*
 * RingBufferWriter.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef RINGBUFFERWRITER_H_
#define RINGBUFFERWRITER_H_

#include <Sink.h>

#include "RingBuffer.h"

namespace hdlc {

class RingBufferWriter: public Sink<uint8_t> {
	RingBuffer<64>& buffer;

public:
	RingBufferWriter(RingBuffer<64>&);
	virtual ~RingBufferWriter();

	virtual bool isReady();
	virtual void write(const uint8_t&);
};

} /* namespace hdlc */

#endif /* RINGBUFFERWRITER_H_ */
