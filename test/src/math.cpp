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
const dual<float> E0{R0}, E{R0, 1}, 
	  Ei = i*E, Ej = j*E, Ek = k*E;

BOOST_AUTO_TEST_CASE(quaternions) {
	BOOST_REQUIRE(i*j == k);
	BOOST_REQUIRE(j*i == -k);
	BOOST_REQUIRE(i*k == -j);
	BOOST_REQUIRE(k*i == j);
	BOOST_REQUIRE(j*k == i);
	BOOST_REQUIRE(k*j == -i);
	BOOST_REQUIRE(i*i == -R1);
	BOOST_REQUIRE(j*j == -R1);
	BOOST_REQUIRE(k*k == -R1);
}

BOOST_AUTO_TEST_CASE(dual_quaternions) {
	BOOST_REQUIRE(E != 0);
	BOOST_REQUIRE(E*E == 0);
	BOOST_REQUIRE(k*E*i == k*i*E);
	BOOST_REQUIRE(k*(E*i) == (k*E)*i);
}
