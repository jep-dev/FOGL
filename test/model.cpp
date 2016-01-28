#include "../inc/model.hpp"

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Model
#include <boost/test/unit_test.hpp>

using namespace Model;

BOOST_AUTO_TEST_CASE(dual_quaternions) {
	BOOST_CHECK(float(dual<>{3,4}) == 5);
}
