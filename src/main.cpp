#include "control.hpp"
#include "main.hpp"
#include "system.hpp"

#include <iostream>

#include "omp.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace System;
using PB = Printer_Base;

int main(int argc, const char **argv) {
	std::atomic_bool alive(true);
	Control::control ctl;
	ctl.run(alive, "share/shade.vert",
			"share/shade.frag");
	return 0;
}
