#include "system.hpp"
#include <iostream>

using namespace System;

int main(int argc, const char **argv) {
	assert(Printer_Base::uni_special(0x80));
	assert(!Printer_Base::uni_special(0x79));
	assert(Printer_Base::strlen("\e[38;5;190mA\e[0m")==1);
	int width = 25;
	char filler = '_';
	double val = 1.234567901234;
	std::cout << Printer_Base::align("Left-aligned", width,
				Printer_Base::LEFT, filler) << "\r\n"
			<< Printer_Base::align("Centered", width,
				Printer_Base::CENTER, filler) << "\r\n"
			<< Printer_Base::align(val, width,
				Printer_Base::CENTER, filler) << "\r\n"
			<< Printer_Base::align(val, width,
				Printer_Base::CENTER, filler, 8) << "\r\n"
			<< Printer_Base::align("Right-aligned", width,
				Printer_Base::RIGHT, filler) << std::endl;
	Printer<4> printer;
	std::string rows[]{"Title","Row title","Row title",""},
		cols[]{"Col 1", "Col 2", "Col 3", "Col 4"};
	int data[]{0, 1, 2, 3, 4, 5, 6, 7};
	printer.push(&rows[0], &rows[0]+4).level();
	printer.push<int, 2, 4>(data, &cols[0], &cols[0]+4).level();
	Printer<6> widePrinter;
	widePrinter.insert(2, printer).level()
		.push<int, 4, 2>(data, &cols[0], &cols[0]+4);
	std::cout << widePrinter << std::endl;
}
