/*
 * FrameBuffer_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "FrameBuffer.h"

using namespace hdlc;

BOOST_AUTO_TEST_CASE( test_FrameBuffer_isEmpty ) {
	FrameBuffer buffer;

	BOOST_CHECK(buffer.isEmpty());

	buffer.endFrame();

	BOOST_CHECK(!buffer.isEmpty());

	buffer.removeFrame();

	BOOST_CHECK(buffer.isEmpty());
}

BOOST_AUTO_TEST_CASE( test_FrameBuffer_isFull ) {
	FrameBuffer buffer;


	for(int i = 0; i < 7; i++) {
		BOOST_CHECK(!buffer.isFull());
		buffer.endFrame();
	}
	BOOST_CHECK(buffer.isFull());

	for(int i = 0; i < 7; i++) buffer.removeFrame();

	for(int i = 0; i < 63; i++) {
		BOOST_CHECK(!buffer.isFull());
		buffer.put(0x00);
	}
	BOOST_CHECK(buffer.isFull());
}

BOOST_AUTO_TEST_CASE( test_FrameBuffer ) {
	FrameBuffer buffer;

	BOOST_CHECK(buffer.size() == 0);

	buffer.put(0x42);
	buffer.endFrame();

	BOOST_CHECK(buffer.size() == 1);

	{
		FrameBuffer::Frame frame = buffer[0];

		BOOST_CHECK(1 == frame.size());
		BOOST_CHECK(0x42 == frame[0]);
	}

	buffer.removeFrame();

	BOOST_CHECK(buffer.size() == 0);

	buffer.put(0x42);
	buffer.revertIncompleteFrame();

	buffer.put(0x42);
	buffer.revertOctets(1);

	buffer.put(0x43);

	buffer.endFrame();

	BOOST_CHECK(buffer.size() == 1);

	{
		FrameBuffer::Frame frame = buffer[0];

		BOOST_CHECK(1 == frame.size());
		BOOST_CHECK(0x43 == frame[0]);
	}
}

BOOST_AUTO_TEST_CASE( test_FrameBuffer_cout ) {
	FrameBuffer buffer;

	buffer.put(0x00); buffer.endFrame();
	buffer.put(0x01); buffer.put(0x02); buffer.endFrame();
	buffer.put(0x03); buffer.put(0x04); buffer.put(0x05); buffer.endFrame();

	std::cout << buffer << std::endl;
}
