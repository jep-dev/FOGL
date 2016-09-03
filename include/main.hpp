#ifndef MAIN_HPP
#define MAIN_HPP

#include <iosfwd>

#define GLFW_INCLUDE_NONE

#include "util.hpp"
#include "system.hpp"
#include "math.hpp"
#include "model.hpp"
#include "view.hpp"
#include "control.hpp"

/**
 * Entry point of the program
 * @param argc Number of args (1+)
 * @param argv Array of args (starting with self)
 * @return Zero if and only if exiting without failure
 */
int main(int argc, const char **argv);

#endif
