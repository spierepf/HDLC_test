/*
 * RingBufferWriter_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include "RingBufferWriter.h"

#include <boost/test/unit_test.hpp>

#include "RingBufferWriter.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_RingBufferWriter ) {
	RingBuffer<64> buffer;
	RingBufferWriter writer(buffer);

	BOOST_CHECK(writer.isReady());
	for(uint8_t i = 0; i < 64; i++) writer.write(i);
	BOOST_CHECK(!writer.isReady());
	BOOST_CHECK_EQUAL(0, buffer.get());
	BOOST_CHECK(writer.isReady());
}

} /* namespace hdlc */
