#ifndef BOOST_TEST_DYN_LINK
#define BOOST_TEST_DYN_LINK
#endif
#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE Math
#endif

#include "math.hpp"
#include "math/quat.hpp"
#include "math/dual.hpp"

#include <iostream>
#include <ostream>
#include <sstream>
#include <boost/test/included/unit_test.hpp>

using namespace Math;

const quat<float> R0{0,0,0,0}, R1{1,0,0,0}, 
	  i{0,1,0,0}, j{0,0,1,0}, k{0,0,0,1};
const dual<float> E0{R0}, E1{R0, 1}, 
	  Ei = i*E1, Ej = j*E1, Ek = k*E1;

BOOST_AUTO_TEST_CASE(quaternions) {
	auto subtrahend = i*j, minuend = k;
	BOOST_REQUIRE_EQUAL((subtrahend - minuend).w, 0);
	auto subtrahend2 = j*i, minuend2 = -k;
	BOOST_REQUIRE_EQUAL((subtrahend2 - minuend2).w, 0);
}

BOOST_AUTO_TEST_CASE(dual_quaternions) {
	auto product = ((E1-E0)*(E1-E0)).u.w;
	BOOST_REQUIRE_EQUAL(product, 0);
}
