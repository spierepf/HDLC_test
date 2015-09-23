/*
 * RingBufferReader.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "RingBufferReader.h"

namespace hdlc {

RingBufferReader::RingBufferReader(RingBuffer<64>& buffer) : buffer(buffer) {
}

RingBufferReader::~RingBufferReader() {
	// TODO Auto-generated destructor stub
}

bool RingBufferReader::isReady() {
	return !buffer.isEmpty();
}

uint8_t RingBufferReader::read() {
	return buffer.get();
}

} /* namespace hdlc */
