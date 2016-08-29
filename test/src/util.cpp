#include "util.hpp"

/// Statically tests traits of derivative types
int main(int argc, const char **argv) {
	using namespace Util;
	// Base types
	typedef pack_t<> T_void;
	typedef pack_t<int> T_I;
	typedef pack_t<float> T_F;
	typedef pack_t<double> T_D;
	typedef pack_t<int, float> T_IF;
	typedef pack_t<int, float, double> T_IFD;
	typedef pack_t<float, double, int> T_FDI;
	T_void V0;
	T_I VI; T_F VF; T_D VD;
	T_IF VIF; T_IFD VIFD;

	// Types with duplicates
	typedef pack_t<int, int> T_II;
	typedef pack_t<float, double, float> T_FDF;
	
	
	// Union types
	typedef typename pack_merge<T_IFD, T_void>::type T_u0;
	typedef typename pack_merge<T_void, T_IFD>::type T_u1;
	typedef typename pack_merge<T_I, T_FDF>::type T_u2;
	static_assert(std::is_same<T_u0, T_IFD>::value, "");
	static_assert(std::is_same<T_u1, T_IFD>::value, "");
	static_assert(std::is_same<T_u2, T_IFD>::value, "");

	// Index of first matching type (or -1)
	static_assert(index_of(T_void {}, int {}) < 0, "");
	static_assert(index_of(T_I {}, int {}) >= 0, "");

	// Index of first matching type (or -1) for each type
	static_assert(std::is_same<decltype(
				indices_of(VIFD, VD + VI + pack_t<char>{})),
			pack_i<2, 0, -1>>::value, "");

	// Prune duplicates
	static_assert(std::is_same<decltype(prune(VIFD)),
			T_IFD>::value, "");
	static_assert(std::is_same<decltype(prune(T_FDF {})),
			decltype(VF+VD)>::value, "");
	
	// Or
	static_assert(std::is_same<decltype(VI+VF+VD),
			T_IFD>::value, "");
	static_assert(!std::is_same<decltype(VD+VF+VI),
			T_IFD>::value, "");
	// Not
	static_assert(std::is_same<decltype(VIF-VF),
			T_I>::value, "");
	static_assert(std::is_same<decltype(VIF-VF-VI),
			T_void>::value, "");
	// Xor
	static_assert(std::is_same<decltype(VI^VI),
			decltype(VI-VI)>::value, "");
	static_assert(std::is_same<decltype(VI^VF),
			decltype(VI+VF)>::value, "");
	// And
	static_assert(std::is_same<decltype(VI & VI),
			T_I>::value, "");
	static_assert(std::is_same<decltype(VIF & VI),
			T_I>::value, "");

	// Permutations
	static_assert(!std::is_same<decltype(VI+VF),
			decltype(VF+VI)>::value, "");
	static_assert(permutes(VI+VF, VF+VI), "");
	static_assert(!permutes(VI+VF, VI), "");

	// Infix operators/types/etc.
	infix_t<pack_t> pack_with;
	infix_t<pack_merge> merge_with;
	infix_t<std::is_same> same_as;

	auto packed1 = 1 <pack_with> 2;
	static_assert(!inner_value(packed1 <same_as> VI), "");
	static_assert(inner_value(prune(packed1) <same_as> VI), "");

	auto packed2 = 3 <pack_with> 4.0f;
	static_assert(inner_value(packed2 <same_as> VIF), "");
	static_assert(!inner_value(3.0f <pack_with> 4.0f <same_as> VIF),"");

	static_assert(inner_value(rotate(T_IFD{}) <same_as> T_FDI{}), "");

	graph_t<T_void, T_void> G_void;
	typedef decltype(G_void + node_t<T_void>{}
		+ node_t<T_I>{} + node_t<T_F>{}) G_verts;

	auto E_01 = edge_t<0,1>{};
	auto E_12 = edge_t<1,2>{};
	auto G_e01 = G_verts {} + E_01;
	static_assert(!contains(G_verts::edges{}, E_01), "");
	static_assert(contains(decltype(G_e01)::edges{}, E_01), "");
	static_assert(!contains(decltype(G_e01)::edges{}, E_12), "");
	static_assert(contains(decltype(G_e01 + E_12)::edges{}, E_12), "");

	auto edge0 = pack_get_t<decltype(G_e01)::edges, 0>::type{};
	static_assert(inner_value(edge0 <same_as> E_01), "");
}
