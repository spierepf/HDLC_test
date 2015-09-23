/*
 * RingBuffer_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "RingBuffer.h"

namespace hdlc {

BOOST_AUTO_TEST_CASE( test_RingBuffer_isEmpty ) {
	RingBuffer<16> ringBuffer;

	BOOST_CHECK(ringBuffer.isEmpty());
	ringBuffer.put(0);
	BOOST_CHECK(!ringBuffer.isEmpty());
	ringBuffer.get();
	BOOST_CHECK(ringBuffer.isEmpty());
}

BOOST_AUTO_TEST_CASE( test_RingBuffer_isFull ) {
	RingBuffer<16> ringBuffer;

	for(int i = 0; i < 15; i++) {
		ringBuffer.put(i);
	}

	BOOST_CHECK(!ringBuffer.isFull());
	ringBuffer.put(15);
	BOOST_CHECK(ringBuffer.isFull());
	ringBuffer.get();
	BOOST_CHECK(!ringBuffer.isFull());
}

BOOST_AUTO_TEST_CASE( test_RingBuffer_put_get ) {
	RingBuffer<16> ringBuffer;

	ringBuffer.put(42);
	BOOST_CHECK(42 == ringBuffer.get());
}

BOOST_AUTO_TEST_CASE( test_RingBuffer_peek ) {
	RingBuffer<16> ringBuffer;

	ringBuffer.put(42);
	BOOST_CHECK(42 == ringBuffer.peek(0));
}

BOOST_AUTO_TEST_CASE( test_RingBuffer_size ) {
	RingBuffer<16> ringBuffer;

	BOOST_CHECK(0 == ringBuffer.size());
	ringBuffer.put(42);
	BOOST_CHECK(1 == ringBuffer.size());
}

} /* namespace hdlc */
