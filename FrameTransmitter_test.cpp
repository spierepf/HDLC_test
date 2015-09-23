/*
 * FrameTransmitter_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "FrameTransmitter.h"

#include "RingBufferWriter.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_ack_zero_by_default ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink, frameBuffer);

	for(int i = 0; i < 5; i++) {
		transmitter.schedule();
		sink.schedule();
	}

	BOOST_CHECK(EscapingSink::FLAG == medium.get());
	BOOST_CHECK((FrameTransmitter::ACK + 0x00) == medium.get());
	uint16_t crc = 0xFFFF;
	crc_ccitt_update(crc, FrameTransmitter::ACK + 0x00);
	crc_ccitt_update(crc, medium.get());
	crc_ccitt_update(crc, medium.get());
	BOOST_CHECK(0x00 == crc);
	BOOST_CHECK(EscapingSink::FLAG == medium.get());
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_ack_frame_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink, frameBuffer);

	transmitter.setAckToSend((SequenceNumber)0x24);

	for(int i = 0; i < 10; i++) {
		transmitter.schedule();
		sink.schedule();
	}

	BOOST_CHECK(EscapingSink::FLAG == medium.get());
	for(int i = 0; i < 2; i++) {
		BOOST_CHECK((FrameTransmitter::ACK + 0x24) == medium.get());
		uint16_t crc = 0xFFFF;
		crc_ccitt_update(crc, FrameTransmitter::ACK + 0x24);
		crc_ccitt_update(crc, medium.get());
		crc_ccitt_update(crc, medium.get());
		BOOST_CHECK(0x00 == crc);
		BOOST_CHECK(EscapingSink::FLAG == medium.get());
	}
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_user_frame_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink, frameBuffer);

	frameBuffer.put(0x42);
	frameBuffer.endFrame();

	for(int i = 0; i < 6; i++) {
		transmitter.schedule();
		sink.schedule();
	}

	BOOST_CHECK(EscapingSink::FLAG == medium.get());
	uint16_t crc = 0xFFFF;
	BOOST_CHECK(0x00 == medium.get());
	crc_ccitt_update(crc, 0x00);
	BOOST_CHECK(0x42 == medium.get());
	crc_ccitt_update(crc, 0x42);
	crc_ccitt_update(crc, medium.get());
	crc_ccitt_update(crc, medium.get());
	BOOST_CHECK(0x00 == crc);
	BOOST_CHECK(EscapingSink::FLAG == medium.get());
}

} /* namespace hdlc */
