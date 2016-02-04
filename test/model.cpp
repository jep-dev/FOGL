#include <string>
#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Model
#include <boost/test/included/unit_test.hpp>

#include "../inc/model.hpp"
using namespace Model;

const quat<float> R0{0}, R1{1}, 
	  i{0,1}, j{0,0,1}, k{0,0,0,1};
const dual<float> E0{R0}, E1{R0, 1}, 
	  Ei = E1*i, Ej = E1*j, Ek = E1*k;

BOOST_AUTO_TEST_CASE(quaternions) {
	BOOST_CHECK_EQUAL(i*j, k);
	BOOST_CHECK_EQUAL(j*i,-k);
}

BOOST_AUTO_TEST_CASE(dual_quaternions) {
	BOOST_CHECK_EQUAL(E1*E1, E0);
}
