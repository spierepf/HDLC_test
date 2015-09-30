/*
 * FrameReceiver_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "FrameReceiver.h"

#include "RingBufferReader.h"
#include "EscapingSource.h"
#include "FrameBuffer.h"
#include "FrameBufferUserFrameHandler.h"
#include "SequenceNumber.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_FrameReceiver_receives_frame ) {
	RingBuffer<64> medium;
	RingBufferReader tmp = RingBufferReader(medium);
	FrameBuffer frameBuffer;
	EscapingSource source = EscapingSource(tmp);
	FrameBufferUserFrameHandler handler(frameBuffer);
	FrameReceiver receiver(source);
	receiver.setFrameHandler(&handler);

	medium.put(EscapingSource::FLAG);	// flag
	uint16_t crc = 0xFFFF;
	medium.put(0x00);					// expected sequence number
	crc_ccitt_update(crc, 0x00);
	medium.put(0x42);		 			// data
	crc_ccitt_update(crc, 0x42);
	medium.put(crc >> 8);				// crc msb
	medium.put(crc & 0xFF);				// crc lsb
	medium.put(EscapingSource::FLAG);	// flag

	while(!medium.isEmpty()) {
		source.schedule();
		receiver.schedule();
	}

	BOOST_CHECK(frameBuffer.size() == 1);
}

BOOST_AUTO_TEST_CASE( test_FrameReceiver_ignores_empty_frames ) {
	RingBuffer<64> medium;
	RingBufferReader tmp = RingBufferReader(medium);
	FrameBuffer frameBuffer;
	EscapingSource source = EscapingSource(tmp);
	FrameBufferUserFrameHandler handler(frameBuffer);
	FrameReceiver receiver(source);
	receiver.setFrameHandler(&handler);

	medium.put(EscapingSource::FLAG); 	// flag
	medium.put(EscapingSource::FLAG); 	// flag

	while(!medium.isEmpty()) {
		source.schedule();
		receiver.schedule();
	}

	BOOST_CHECK(frameBuffer.size() == 0);
}

BOOST_AUTO_TEST_CASE( test_FrameReceiver_ignores_frames_with_bad_crc ) {
	RingBuffer<64> medium;
	RingBufferReader tmp = RingBufferReader(medium);
	FrameBuffer frameBuffer;
	EscapingSource source = EscapingSource(tmp);
	FrameBufferUserFrameHandler handler(frameBuffer);
	FrameReceiver receiver(source);
	receiver.setFrameHandler(&handler);

	medium.put(EscapingSource::FLAG);	// flag
	uint16_t crc = 0xFFFF;
	medium.put(0x00);					// expected sequence number
	crc_ccitt_update(crc, 0x00);
	medium.put(0x42);		 			// data
	crc_ccitt_update(crc, 0x42);
	crc ^= 0x01;						// corrupted crc
	medium.put(crc >> 8);				// crc msb
	medium.put(crc & 0xFF);				// crc lsb
	medium.put(EscapingSource::FLAG);	// flag

	while(!medium.isEmpty()) {
		source.schedule();
		receiver.schedule();
	}

	BOOST_CHECK(frameBuffer.size() == 0);
}

BOOST_AUTO_TEST_CASE( test_FrameReceiver_receives_multiple_frames ) {
	RingBuffer<64> medium;
	RingBufferReader tmp = RingBufferReader(medium);
	FrameBuffer frameBuffer;
	EscapingSource source = EscapingSource(tmp);
	FrameBufferUserFrameHandler handler(frameBuffer);
	FrameReceiver receiver(source);
	receiver.setFrameHandler(&handler);
	uint8_t count = 7;

	for(int i = 0; i < count; i++) {
		medium.put(EscapingSource::FLAG);	// flag
		uint16_t crc = 0xFFFF;
		medium.put(0x00 + i);				// expected sequence number
		crc_ccitt_update(crc, 0x00 + i);
		medium.put(i);		 				// data
		crc_ccitt_update(crc, i);
		medium.put(crc >> 8);				// crc msb
		medium.put(crc & 0xFF);				// crc lsb
		medium.put(EscapingSource::FLAG);	// flag
	}

	while(!medium.isEmpty()) {
		source.schedule();
		receiver.schedule();
	}

	BOOST_CHECK(frameBuffer.size() == count);
}

class AckFrameHandler : public FrameHandler {
public:
	SequenceNumber lastAckReceived;

	virtual void handle(const uint8_t header, const uint8_t* payload, const uint8_t payloadSize) {
		lastAckReceived = header;
	}
};

BOOST_AUTO_TEST_CASE( test_FrameReceiver_recognizes_ack_frames ) {
	RingBuffer<64> medium;
	RingBufferReader tmp = RingBufferReader(medium);
	EscapingSource source = EscapingSource(tmp);
	AckFrameHandler handler;
	FrameReceiver receiver(source);
	receiver.setFrameHandler(&handler);

	medium.put(EscapingSource::FLAG);		// flag
	uint16_t crc = 0xFFFF;
	medium.put(FrameReceiver::ACK + 0x24);	// ack with sequence number
	crc_ccitt_update(crc, FrameReceiver::ACK + 0x24);
	medium.put(crc >> 8);					// crc msb
	medium.put(crc & 0xFF);					// crc lsb
	medium.put(EscapingSource::FLAG);		// flag

	while(!medium.isEmpty()) {
		source.schedule();
		receiver.schedule();
	}

	BOOST_CHECK(0x24 == handler.lastAckReceived);
}

} /* namespace hdlc */
