#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif
#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE System
#endif

#include "system.hpp"
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(all) {
	using namespace System;
	BOOST_REQUIRE(Printer_Base::uni_special(0xc0));
	BOOST_REQUIRE(!Printer_Base::uni_special(0xb9));
}
