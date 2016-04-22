#include "util.hpp"
#include "model.hpp"
#include "view.hpp"
#include "system.hpp"

#include "math.hpp"

#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <iomanip>
#include <string>
#include <thread>
#include <typeinfo>

using namespace Model;
using namespace System;
using namespace Detail;
using std::cout;
using std::ostream;
using std::endl;
using PB = Printer_Base;
using R = double;
using DU = dual<R>;
using CDU = const dual<R>;
using QU = quat<R>;
using CQU = const quat<R>;
using CC = const char *;

template<int W1, int W2, int... WN,
	typename T1, typename T2, typename... TN>
std::string columns(const pack_i<W1, W2, WN...>,
		T1 && t1, T2 && t2, TN &&... tn) {
	return columns(pack_i<W1> {}, SELF(t1)) +
		columns(pack_i<W2, WN...> {}, FWD(T2,t2), FWD(TN,tn)...);
}
template<int W1, typename T1, typename T2, typename... TN>
std::string columns(const pack_i<W1> p,
		T1 && t1, T2 && t2, TN &&... tn) {
	return columns(pack_i<W1> {}, SELF(t1)) +
		columns(pack_i<W1> {}, FWD(T2,t2), FWD(TN,tn)...);
}
template<int W1, typename T1>
std::string columns(const pack_i<W1>, T1 && t1) {
	std::ostringstream oss;
	oss << std::setw(W1) << SELF(t1);
	return oss.str();
}

template<int I1, int I2, int... IN>
ostream& operator<<(ostream &out, const pack_i<I1, I2, IN...>)
	{return out << I1 << ", " << pack_i<I2, IN...> {};}
template<int I1>
ostream& operator<<(ostream &out, const pack_i<I1>)
	{return out << I1;}
ostream& operator<<(ostream &out, const pack_i<>)
	{return out;}

int main(int argc, const char **argv) {
	static_assert(std::is_pod<QU>::value, "POD test");
	static_assert(std::is_pod<DU>::value, "POD test");

	// Base types
	typedef pack_t<> T_void;
	typedef pack_t<int> T_I;
	typedef pack_t<float> T_F;
	typedef pack_t<double> T_D;
	typedef pack_t<int, float> T_IF;
	typedef pack_t<int, float, double> T_IFD;
	T_void V0;
	T_I VI; T_F VF; T_D VD;
	T_IF VIF; T_IFD VIFD;
	
	
	// Concatenation types
	static_assert(std::is_same<typename
			pack_cat<float, T_I>::type, T_IF>::value, "");
	static_assert(std::is_same<typename
			pack_cat<int, T_I>::type, T_I>::value, "");

	// Union types
	typedef pack_t<float, double, float> T_FDF;
	typedef typename pack_merge<T_IFD, T_void>::type T_u0;
	typedef typename pack_merge<T_void, T_IFD>::type T_u1;
	typedef typename pack_merge<T_FDF, T_I>::type T_u2;
	static_assert(std::is_same<T_u0, T_IFD>::value, "");
	static_assert(std::is_same<T_u1, T_IFD>::value, "");
	static_assert(std::is_same<T_u2, T_IFD>::value, "");

	// Index of first matching type (or -1)
	static_assert(index_of(T_void {}, int {}) < 0, "");
	static_assert(index_of(T_I {}, int {}) >= 0, "");

	// Index of first matching type (or -1) for each type
	static_assert(std::is_same<
			decltype(indices_of(VIFD, VD + VI + pack_t<char>{})),
			pack_i<2, 0, -1>>::value, "");

	// Prune duplicates
	static_assert(std::is_same<decltype(prune(T_FDF {})), decltype(VF+VD)>::value, "");
	static_assert(std::is_same<decltype(prune(VIFD)), T_IFD>::value, "");
	
	// Or
	static_assert(std::is_same<decltype(VI+VF+VD), T_IFD>::value, "");
	static_assert(!std::is_same<decltype(VD+VF+VI), T_IFD>::value, "");
	// Not
	static_assert(std::is_same<decltype(VIF-VF), T_I>::value, "");
	static_assert(std::is_same<decltype(VIF-VF-VI), T_void>::value, "");
	// Xor
	static_assert(std::is_same<decltype(VI^VI), decltype(VI-VI)>::value, "");
	static_assert(std::is_same<decltype(VI^VF), decltype(VI+VF)>::value, "");
	// And
	static_assert(std::is_same<decltype(VI & VI), T_I>::value, "");
	static_assert(std::is_same<decltype(VIF & VI), T_I>::value, "");

	// Permutations
	static_assert(!std::is_same<decltype(VI+VF), decltype(VF+VI)>::value, "");
	static_assert(permutes(VI+VF, VF+VI), "");
	static_assert(!permutes(VI+VF, VI), "");

	// Infix operators/types/etc.
	infix_t<pack_t> pack_with;
	infix_t<pack_merge> merge_with;
	infix_t<std::is_same> same_as;

	auto packed1 = 1 <pack_with> 2;
	static_assert(!inner_value(inner_type(packed1 <same_as> VI)), "");
	static_assert(inner_value(inner_type(prune(packed1) <same_as> VI)), "");

	/*static CDU i = {0,1}, j = {0,0,1}, k = i*j,
		E = {0,0,0,0,1}, iE = i*E, jE = j*E, kE = k*E,
		units[]{1, i, j, k, E, iE, jE, kE};
	typedef R S1;
	typedef S1 T1;
	Impulse<R,S1,T1> gravity {[]
		(const State<S,T> &lhs, R t)
			{return R(-9.8);}
		}, spring {[]
		(const State<S1,T1> &lhs, R t) {
			static const R k = 10, b = 1;
			return lhs.u*-k + lhs.v*-b;
		}};

	State<S1,T1> particle {0.0, 0.0};

	static constexpr const char 
		*c0 = "\e[38;5;215m", 
		*cEnd = "\e[0m";

	static constexpr auto pk10 = pack_i<10> {};
	static constexpr auto pk4 = pack_i<4> {};
	static constexpr auto pk3 = pack_i<3> {};

	auto printState = [=] (const State<S1,T1> &state) {
		std::ostringstream oss;
		oss << "u = " << c0 << columns(pk10, state.u) << cEnd
			<< ", v = " << c0 << columns(pk10, state.v) << cEnd;
		return oss.str();
	};

	int N = 100, M = 10;//N/10;
	R t = 0, dt = 1.0/N;
	for(int i = 0; i <= N; ++i) {

		std::function<S1 (const S1&, const S1&)>
			add_s = [] (const S1 &s1, const S1 &s2)
				-> S {return s1 + s2;};

		std::function<T(const T&, const T&)>
			add_t = [] (const T &t1, const T &t2)
				-> T {return t1 + t2;};

		std::function<R& (R&, const R&)>
			inc = [] (R &r1, const R &r2)
				-> R& {return r1 += r2;};

		auto rk4 = integrate(gravity, particle,
			add_s, add_t, inc, t, dt);

		if(i%M == 0) {
			cout << "   " << columns(pack_i<3>{}, int(100*i/N))
				<< "%; t = " << columns(pk3, t)
				<< "s, dt = " << columns(pk3, dt)
				<< "\r\n     " << printState(particle)
				<< "\r\n  -> " << printState(rk4)
				<< "\r\n" << endl;
		}
		particle = rk4;
	}
	cout << endl;

	auto printCamera = [=] (const State<DU> &state) {
		std::ostringstream oss;
		oss << "  u = " << columns(pk10, state.u)
			<< ", v = " << columns(pk10, state.v) << endl;
		return oss.str();
	};
	t = 0;*/

	//#pragma message("All tests passed.")	
	
	std::atomic_bool alive(true);
	auto delay = std::chrono::milliseconds(150);
	
	auto quit = [&alive]() {
		alive = false;
	};
	auto updateView = []() {
		return true;
	};

	// Update model based on state & delay
	auto updateModel = [&alive, &delay]{
		static int t = 0;
		while(alive) {
			std::cout << "Model update #" << t++ << std::endl;
			std::this_thread::sleep_for(delay);
		}
	};

	View::view display;
	if(display.valid) {
		std::thread modelThread(updateModel);
		display.run(updateView, quit);
		modelThread.join();
	}

	glfwTerminate();

	return 0;
}
