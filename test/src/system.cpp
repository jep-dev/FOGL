#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE System

#include "system.hpp"
#include <boost/test/included/unit_test.hpp>

using namespace System;

BOOST_AUTO_TEST_CASE(uni_special) {
	BOOST_REQUIRE(Printer_Base::uni_special(0x80));
	BOOST_REQUIRE(!Printer_Base::uni_special(0x79));
}

BOOST_AUTO_TEST_CASE(uni_strlen) {
	BOOST_REQUIRE_EQUAL(Printer_Base::uni_strlen("A"),1);
}

BOOST_AUTO_TEST_CASE(strlen_) {
	BOOST_REQUIRE_EQUAL(Printer_Base::strlen("\e[38;5;190mA\e[0m"),1);
}
