#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Math

#include "math.hpp"

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
	BOOST_REQUIRE(i*j == k);
	BOOST_REQUIRE(j*i == -k);
	BOOST_REQUIRE(i*i == -R1);
	BOOST_REQUIRE(j*j == -R1);
	BOOST_REQUIRE(k*k == -R1);
}

BOOST_AUTO_TEST_CASE(dual_quaternions) {
	BOOST_REQUIRE(E1 != 0);
	BOOST_REQUIRE(E1*E1 == 0);
	BOOST_REQUIRE((k*E1)*i != i*(k*E1));
}
