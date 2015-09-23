/*
 * RingBufferReader_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "RingBufferReader.h"

#include <boost/test/unit_test.hpp>

#include "RingBufferReader.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_RingBufferReader ) {
	RingBuffer<64> buffer;
	RingBufferReader reader(buffer);

	BOOST_CHECK(!reader.isReady());
	buffer.put(0x42);
	BOOST_CHECK(reader.isReady());
	BOOST_CHECK_EQUAL(0x42, reader.read());
	BOOST_CHECK(!reader.isReady());
}

} /* namespace hdlc */
