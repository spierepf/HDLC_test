/*
 * RingBufferWriter.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "RingBufferWriter.h"

namespace hdlc {

RingBufferWriter::RingBufferWriter(RingBuffer<64>& buffer) : buffer(buffer) {
}

RingBufferWriter::~RingBufferWriter() {
	// TODO Auto-generated destructor stub
}

bool RingBufferWriter::isReady() {
	return !buffer.isFull();
}

void RingBufferWriter::write(const uint8_t& b) {
	buffer.put(b);
}

} /* namespace hdlc */
