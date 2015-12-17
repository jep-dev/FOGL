#include "../inc/main.hpp"
#include "../inc/control.hpp"
#include "../inc/view.hpp"

#include "../inc/rijk.hpp"
#include <boost/math/quaternion.hpp>

#include <algorithm>
#include <vector>
#include <utility>

#include <iostream>
#include <cmath>
#include <omp.h>


using rq = rijk<float>;
using bq = boost::math::quaternion<float>;

bq to_boost(rq const& from) {
	return bq(from[0], from[1], from[2], from[3]);
}
rq to_rijk(bq const& from) {
	return rq(from.R_component_1(), from.R_component_2(),
			from.R_component_3(), from.R_component_4());
}

bool eq(rq const& lhs, bq const& rhs) {
	return to_boost(lhs) == rhs;
}

void failed(std::pair<rq,bq> const& p) {
		std::cout << to_boost(p.first) 
			<< (eq(p.first, p.second) ? " == " : " =/= ")
			<< p.second << std::endl;
}

void testValidity(void) {
	rq a(1, 0, 0, 0), 
	   i(0, 1, 0, 0),
	   j(0, 0, 1, 0),
	   k(0, 0, 0, 1);

	std::vector<std::pair<rq,bq>> tests = {
		{a, to_boost(i)}, // guarantee neq
		{a, to_boost(a)}, // guarantee eq
		{a+i, to_boost(a)+to_boost(i)}, // add
		{i*j, to_boost(i)*to_boost(j)}, // mul
		{i*i, to_boost(i)*to_boost(i)}, // cyc
		{to_rijk(to_boost(a)), to_boost(a)}, // trans
		{a/i, to_boost(a)/to_boost(i)}, // div, inverse cyc
		{a*2.0f, to_boost(a)*2.0f}, // scalar mul
		{i*j*i*k, to_boost(i)} // chained mul
	};
	std::for_each(tests.begin(), tests.end(), &failed);
}

void testTiming(void) {
	const unsigned int steps = 100000, param = 19;
	double dtR = 0, dtB = 0;
	rq rResult;
	bq bResult;
	float start = 1,
		  step = 1+1/(float)(1<<param),
		  actual = start*pow(step,steps);
	std::cout << "(1+1/2^" << param << ") ^ -" 
		<< steps << " = " << actual << std::endl;
	omp_set_num_threads(2);
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			rq rStep(step, 0, 0, 0), 
			   rVal(start, 0, 0, 0);
			double t1, t0 = omp_get_wtime();
			for(int i = 0; i < steps; i++) {
				rVal = rVal * rStep;
			}
			t1 = omp_get_wtime() - t0;
			#pragma omp critical
			{
				rResult = rVal;
				dtR = t1;
			}
		}
		#pragma omp section
		{
			bq bStep(step, 0, 0, 0), 
			   bVal(start, 0, 0, 0);
			double t1, t0 = omp_get_wtime();
			for(int i = 0; i < steps; ++i) {
				bVal = bVal * bStep;
			}
			t1 = omp_get_wtime() - t0;
			#pragma omp critical
			{
				bResult = bVal;
				dtB = t1;
			}
		}
	}
	bq bAct = bq(actual, 0, 0, 0);
	double dtRatio = 100.0f*dtR/dtB,
		dxR = abs(to_boost(rResult) - bAct),
		dxB = abs(bResult - bAct);
	const char *br = "\r\n";
	std::cout
		<< "100% * dt_Rijk / dt_Boost = " 
		<< dtRatio << "%" << br
		<< "               |err_Rijk| = " << dxR << br
		<< "              |err_Boost| = " << dxB << br
		<< "(" << steps << " steps)" << std::endl;
}

int main(int argc, const char **argv) {
	std::cout.precision(15);
	//testValidity();
	testTiming();


/*	const unsigned int reps = 1;


	std::cout.precision(15);
	float stepw = 0.00001f;

	double dtBoost, dtRijk,
		   errorBoost, errorRijk;

	boost::math::quaternion<float>
		step(stepw, 0, 0, 0);
	float measuredBoost, measuredRijk, 
		  actual = pow(stepw, reps);

	// Exactly one thread per test
	#pragma omp parallel num_threads(2)
	{
		double t0 = omp_get_wtime();
		switch(omp_get_thread_num()) {

		*/
	return 0;
}
