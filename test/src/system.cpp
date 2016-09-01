#include "system.hpp"
#include <iostream>

using namespace System;

int main(int argc, const char **argv) {
	assert(Printer_Base::uni_special(0x80));
	assert(!Printer_Base::uni_special(0x79));
	assert(Printer_Base::strlen("\e[38;5;190mA\e[0m")==1);
	int width = 25;
	std::cout << Printer_Base::align("Left-aligned", width,
			Printer_Base::LEFT) << std::endl;
	std::cout << Printer_Base::align("Centered", width,
			Printer_Base::CENTER) << std::endl;
	std::cout << Printer_Base::align("Right-aligned", width,
			Printer_Base::RIGHT) << std::endl;
	std::cout << Printer_Base::align(1.2345, width,
			Printer_Base::CENTER) << std::endl;
}

/*BOOST_AUTO_TEST_CASE(uni_special) {
	BOOST_REQUIRE(Printer_Base::uni_special(0x80));
	BOOST_REQUIRE(!Printer_Base::uni_special(0x79));
}

BOOST_AUTO_TEST_CASE(uni_strlen) {
	BOOST_REQUIRE_EQUAL(Printer_Base::uni_strlen("A"),1);
}

BOOST_AUTO_TEST_CASE(strlen_) {
	BOOST_REQUIRE_EQUAL(Printer_Base::strlen("\e[38;5;190mA\e[0m"),1);
}*/
