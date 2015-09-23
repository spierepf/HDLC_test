/*
 * SequenceNumber_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "SequenceNumber.h"

BOOST_AUTO_TEST_CASE( test_SequenceNumber_integer_cast ) {
	hdlc::SequenceNumber x = 5;

	BOOST_CHECK(5 == x);
}

BOOST_AUTO_TEST_CASE( test_SequenceNumber_preincrement ) {
	hdlc::SequenceNumber x;

	for(int i = 1; i <= 63; i++) {
		BOOST_CHECK(i == ++x);
	}
	BOOST_CHECK(0 == ++x);
}

BOOST_AUTO_TEST_CASE( test_SequenceNumber_equality ) {
	hdlc::SequenceNumber x = 1;
	hdlc::SequenceNumber y = 1;
	hdlc::SequenceNumber z = 2;

	BOOST_CHECK(x == y);
	BOOST_CHECK(!(x == z));
	BOOST_CHECK(!(z == x));
}
