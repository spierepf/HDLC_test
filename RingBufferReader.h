/*
 * RingBufferReader.h
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#ifndef RINGBUFFERREADER_H_
#define RINGBUFFERREADER_H_

#include <Source.h>

#include "RingBuffer.h"

namespace hdlc {

class RingBufferReader: public Source<uint8_t> {
	RingBuffer<64>& buffer;

public:
	RingBufferReader(RingBuffer<64>&);
	virtual ~RingBufferReader();

	virtual bool isReady();
	virtual uint8_t read();
};

} /* namespace hdlc */

#endif /* RINGBUFFERREADER_H_ */
