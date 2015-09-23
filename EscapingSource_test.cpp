/*
 * EscapingSource_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "EscapingSource.h"

#include "RingBufferReader.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_EscapingSource_is_born_unready ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	BOOST_CHECK(!escapingSource.isReady());
}

BOOST_AUTO_TEST_CASE( test_EscapingSource_becomes_ready_after_receiving ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	medium.put(0x42);

	escapingSource.schedule();

	BOOST_CHECK(escapingSource.isReady());
}

BOOST_AUTO_TEST_CASE( test_EscapingSource_becomes_unready_after_next ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	medium.put(0x42);

	escapingSource.schedule();
	escapingSource.next();

	BOOST_CHECK(!escapingSource.isReady());
}

BOOST_AUTO_TEST_CASE( test_EscapingSource_receives_octet_correctly ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	medium.put(0x42);

	escapingSource.schedule();
	BOOST_CHECK(!escapingSource.isFlag());
	BOOST_CHECK(0x42 == escapingSource.read());
}

BOOST_AUTO_TEST_CASE( test_EscapingSource_receives_escaped_octet_correctly ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	medium.put(EscapingSource::ESCAPE);
	medium.put(EscapingSource::FLAG ^ EscapingSource::MASK);

	medium.put(EscapingSource::ESCAPE);
	medium.put(EscapingSource::ESCAPE ^ EscapingSource::MASK);

	escapingSource.schedule();
	escapingSource.schedule();
	BOOST_CHECK(!escapingSource.isFlag());
	BOOST_CHECK(EscapingSource::FLAG == escapingSource.read());
	escapingSource.next();

	escapingSource.schedule();
	escapingSource.schedule();
	BOOST_CHECK(!escapingSource.isFlag());
	BOOST_CHECK(EscapingSource::ESCAPE == escapingSource.read());
}

BOOST_AUTO_TEST_CASE( test_EscapingSource_receives_flag_octet_correctly ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	medium.put(EscapingSource::FLAG);

	escapingSource.schedule();

	BOOST_CHECK(escapingSource.isFlag());
}

BOOST_AUTO_TEST_CASE( test_EscapingSource_receives_regular_octet_correctly_after_receiving_flag ) {
	RingBuffer<64> medium;
	RingBufferReader source = RingBufferReader(medium);
	EscapingSource escapingSource = EscapingSource(source);

	medium.put(EscapingSource::FLAG);
	medium.put(0x42);

	escapingSource.schedule();

	BOOST_CHECK(escapingSource.isFlag());
	escapingSource.read();
	escapingSource.next();

	escapingSource.schedule();

	BOOST_CHECK(!escapingSource.isFlag());
	BOOST_CHECK(0x42 == escapingSource.read());
}

} /* namespace hdlc */
