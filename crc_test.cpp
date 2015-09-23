/*
 * crc_test.cpp
 *
 *  Created on: Sep 22, 2015
 *      Author: peter
 */

#include <boost/test/unit_test.hpp>

#include "crc.h"

using namespace hdlc;

BOOST_AUTO_TEST_CASE( test_crc_ccitt_update ) {
	uint16_t crc = 0xFFFF;

	crc_ccitt_update(crc, 0x42);

	BOOST_CHECK(0x8976 == crc);

	crc_ccitt_update(crc, 0x89);
	crc_ccitt_update(crc, 0x76);

	BOOST_CHECK(0x0000 == crc);
}

