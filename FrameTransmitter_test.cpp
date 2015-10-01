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

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_is_born_ready ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	BOOST_CHECK(transmitter.isReady());
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_becomes_unready_when_transmitting ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	transmitter.transmit(FrameTransmitter::ACK + 0x42);

	BOOST_CHECK(!transmitter.isReady());
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_nothing_by_default ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	for(int i = 0; i < 5; i++) {
		transmitter.schedule();
		sink.schedule();
	}

	BOOST_CHECK(0 == medium.size());
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_ack_frame_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	transmitter.transmit(FrameTransmitter::ACK + 0x24);

	for(int i = 0; i < 5; i++) {
		transmitter.schedule();
		sink.schedule();
	}

	BOOST_CHECK(EscapingSink::FLAG == medium.get());
	BOOST_CHECK((FrameTransmitter::ACK + 0x24) == medium.get());
	uint16_t crc = 0xFFFF;
	crc_ccitt_update(crc, FrameTransmitter::ACK + 0x24);
	crc_ccitt_update(crc, medium.get());
	crc_ccitt_update(crc, medium.get());
	BOOST_CHECK(0x00 == crc);
	BOOST_CHECK(EscapingSink::FLAG == medium.get());
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_two_frames_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	for(int j = 0; j < 2; j++) {
		transmitter.transmit(FrameTransmitter::ACK + 0x24);
		for(int i = 0; i < 5; i++) {
			transmitter.schedule();
			sink.schedule();
		}

		BOOST_CHECK(EscapingSink::FLAG == medium.get());
		BOOST_CHECK((FrameTransmitter::ACK + 0x24) == medium.get());
		uint16_t crc = 0xFFFF;
		crc_ccitt_update(crc, FrameTransmitter::ACK + 0x24);
		crc_ccitt_update(crc, medium.get());
		crc_ccitt_update(crc, medium.get());
		BOOST_CHECK(0x00 == crc);
		BOOST_CHECK(EscapingSink::FLAG == medium.get());
	}
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_becomes_ready_only_once_transmission_complete ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	transmitter.transmit(FrameTransmitter::ACK + 0x24);

	for(int i = 0; i < 5; i++) {
		BOOST_CHECK(!transmitter.isReady());
		transmitter.schedule();
		sink.schedule();
	}

	BOOST_CHECK(transmitter.isReady());
}

BOOST_AUTO_TEST_CASE( test_FrameTransmitter_transmits_user_frame_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter tmp = RingBufferWriter(medium);
	FrameBuffer frameBuffer;
	EscapingSink sink = EscapingSink(tmp);

	FrameTransmitter transmitter(sink);

	frameBuffer.put(0x42);
	frameBuffer.endFrame();

	transmitter.transmit(0x00, frameBuffer[0]);

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
