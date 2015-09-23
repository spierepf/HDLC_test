/*
 * EscapingSink_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "EscapingSink.h"

#include "RingBufferWriter.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_EscapingSink_is_born_ready ) {
	RingBuffer<64> medium;
	RingBufferWriter sink = RingBufferWriter(medium);
	EscapingSink escapingSink = EscapingSink(sink);

	BOOST_CHECK(escapingSink.isReady());
}

BOOST_AUTO_TEST_CASE( test_EscapingSink_becomes_unready_when_transmitting ) {
	RingBuffer<64> medium;
	RingBufferWriter sink = RingBufferWriter(medium);
	EscapingSink escapingSink = EscapingSink(sink);

	escapingSink.write(0x42);
	BOOST_CHECK(!escapingSink.isReady());
}

BOOST_AUTO_TEST_CASE( test_EscapingSink_becomes_ready_after_transmitting ) {
	RingBuffer<64> medium;
	RingBufferWriter sink = RingBufferWriter(medium);
	EscapingSink escapingSink = EscapingSink(sink);

	escapingSink.write(0x42);

	escapingSink.schedule();

	BOOST_CHECK(escapingSink.isReady());
}

BOOST_AUTO_TEST_CASE( test_EscapingSink_transmits_octet_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter sink = RingBufferWriter(medium);
	EscapingSink escapingSink = EscapingSink(sink);

	escapingSink.write(0x42);

	escapingSink.schedule();

	BOOST_CHECK(0x42 == medium.get());
	BOOST_CHECK(medium.isEmpty());
}

BOOST_AUTO_TEST_CASE( test_EscapingSink_transmits_escapable_octet_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter sink = RingBufferWriter(medium);
	EscapingSink escapingSink = EscapingSink(sink);

	escapingSink.write(EscapingSink::FLAG);
	escapingSink.schedule();
	escapingSink.write(EscapingSink::ESCAPE);
	escapingSink.schedule();

	BOOST_CHECK(EscapingSink::ESCAPE == medium.get());
	BOOST_CHECK((EscapingSink::FLAG ^ EscapingSink::MASK) == medium.get());
	BOOST_CHECK(EscapingSink::ESCAPE == medium.get());
	BOOST_CHECK((EscapingSink::ESCAPE ^ EscapingSink::MASK) == medium.get());
	BOOST_CHECK(medium.isEmpty());
}

BOOST_AUTO_TEST_CASE( test_EscapingSink_transmits_flag_octet_correctly ) {
	RingBuffer<64> medium;
	RingBufferWriter sink = RingBufferWriter(medium);
	EscapingSink escapingSink = EscapingSink(sink);

	escapingSink.writeFlag();
	escapingSink.schedule();

	BOOST_CHECK(EscapingSink::FLAG == medium.get());
	BOOST_CHECK(medium.isEmpty());
}

} /* namespace hdlc */
