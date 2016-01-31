#include <string>
#include <iostream>

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Model
#include <boost/test/included/unit_test_framework.hpp>

#include "../inc/model.hpp"
using namespace Model;

BOOST_AUTO_TEST_CASE(dual_quaternions) {
	dual<float> A,B,
		one(1), 
		i(0,1), 
		j(0,0,1),
		k(0,0,0,1);

	BOOST_TEST_CONTEXT("A = " << (A = i) << ";   B = " << (B = one+k)) {
		BOOST_CHECK(float(A) == 1);
		BOOST_CHECK(A*j == k);
		BOOST_CHECK(j*A ==-k);
		BOOST_CHECK(float(B) == float(sqrt(2)));
	}
}
