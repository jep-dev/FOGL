#ifndef UTIL_HPP
#define UTIL_HPP

#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <functional>

#ifndef FWD
#define FWD(X,Y) std::forward<X>(Y)
#endif

#ifndef SELF
#define SELF(Y) std::forward<decltype(Y)>(Y)
#endif

namespace Detail {
	// util/types
	struct undef_t;
	struct delim_tag;
	template<typename...> struct pack_t;
	template<int...> struct pack_i;
	template<typename...> struct set_t;
	template<typename T> struct node;
	template<typename U, typename V> struct edge;
	template<typename V, typename E> struct graph;

	// util/functional
	template<typename T> struct Signature;
	template<int, int, typename...> struct ForSeq;
	template<int, typename...> struct ForAll;
	template<int SEQ, typename FN, typename T1, typename... TN>
	struct TransformForAll;
	template<int, int, typename...> struct ForZip;

}
#include "util/types.hpp"

// TODO -- swap with Boost version
template<typename B>
struct Castable {
	virtual operator B(void) const = 0;
};

template<typename... S>
void zipper(std::function<void(S...)> fn) {}

template<typename S1, typename... SN, typename T1, typename... TN,
	template<typename> class FN = std::function>
void zipper(FN<void(S1, SN...)> fn,
		S1 && s1, SN &&... sn,
		T1 && t1, TN &&... tn) {
	fn(SELF(s1), SELF(sn)...);
	zipper(fn, SELF(t1), SELF(tn)...);
}


/*-----------------------------------------------------------------------*/

namespace Detail {

	template<typename T>
	struct Signature {
		typedef decltype(&T::operator()) call_type;
		typedef Signature<call_type> sig_type;
		typedef typename sig_type::result_type result_type;
		typedef typename sig_type::args_type args_type;
	};
	template<typename R, typename S, typename... T>
	struct Signature<R (S::*) (T...) const> {
		typedef R result_type;
		typedef typename std::tuple<T...> args_type;
	};
	template<typename R, typename S, typename... T>
	struct Signature<R& (S::*) (T &&...)> {
		typedef R& result_type;
		typedef typename std::tuple<T...> args_type;
	};
	template<typename R, typename... T>
	struct Signature<R (*) (T...)> {
		typedef R result_type;
		typedef typename std::tuple<T...> args_type;
	};
	

	template<int SEQ, int CUR, typename FN, 
		typename T1, int N1, typename... TN>
	struct ForSeq<SEQ, CUR, FN, T1(&)[N1], TN...> {
		typedef ForSeq<SEQ/N1, CUR-1, FN, TN..., T1> NEXT;
		typedef typename NEXT::result_type result_type;
		static result_type apply(FN fn, T1 (&t1)[N1], TN &&... tn) {
			return NEXT::apply(fn, FWD(TN,tn)..., FWD(T1,t1[SEQ%N1]));
		}
	};

	template<int SEQ, typename FN, typename... TN>
	struct ForSeq<SEQ, 0, FN, TN...> {
		static auto apply(FN fn, TN &&... tn)
				-> decltype(fn(FWD(TN,tn)...)) {
			return fn(FWD(TN,tn)...);
		}
		typedef typename Signature<decltype(&apply)>
			::result_type result_type;
	};

	template<int SEQ, typename FN, typename T1, typename... TN>
	struct ForAll<SEQ, FN, T1, TN...> {
		typedef ForAll<SEQ-1, FN, T1, TN...> NEXT;
		typedef ForSeq<SEQ-1, 1+sizeof...(TN), FN, T1, TN...> THIS;
		static void apply(FN fn, T1 && t1, TN &&... tn) {
			NEXT::apply(fn, SELF(t1), SELF(tn)...);
			THIS::apply(fn, SELF(t1), SELF(tn)...);
		}
	};
	template<typename FN, typename T1, typename... TN>
	struct ForAll<0, FN, T1, TN...> {
		static void apply(FN fn, T1 && t1, TN &&... tn) {}
	};

	template<int SEQ, typename FN, typename T1, typename... TN>
	struct TransformForAll {
		static constexpr int SIZE = Sizes<T1, TN..., delim_tag>::SIZE;
		typedef TransformForAll<SEQ-1, FN, T1, TN...> NEXT;
		typedef ForSeq<SEQ-1, 1+sizeof...(TN), FN, T1, TN...> THIS;
		typedef typename THIS::result_type result_type;
		static void apply(FN fn, result_type (&out)[SIZE],
				T1 && t1, TN &&... tn) {
			NEXT::apply(fn, FWD(result_type (&)[SIZE], out), 
					FWD(T1,t1), FWD(TN,tn)...);
			out[SEQ-1] = THIS::apply(fn, FWD(T1,t1), FWD(TN,tn)...);
		}
	};

	template<typename FN, typename T1, typename... TN>
	struct TransformForAll<0, FN, T1, TN...> {
		template<typename RET>
		static void apply(FN fn, RET && out, T1 && t1, TN &&... tn) {}
	};

	template<int SEQ, int N, typename FN, typename T1, typename... TN>
	struct ForZip<SEQ, N, FN, T1, TN...> {
		typedef ForZip<SEQ-1, N, FN, T1, TN...> NEXT;
		static void apply(FN fn, T1 (&t1)[N], TN (&...tn)[N]) {
			NEXT::apply(fn, FWD(T1 (&)[N],t1), FWD(TN(&)[N],tn)...);
			fn(t1[SEQ-1], tn[SEQ-1]...);
		}
	};
	template<int N, typename FN, typename T1, typename... TN>
	struct ForZip<0, N, FN, T1, TN...> {
		static void apply(FN fn, T1 (&t1)[N], TN (&...tn)[N]) {}
	};
}

template<typename T1, typename... TN>
constexpr int SizeProduct(void) {
	return Detail::Sizes<T1, TN..., Detail::delim_tag>::SIZE;
}
template<typename T1, typename... TN>
constexpr int SizeProduct(T1 && t1, TN &&... tn) {
	return SizeProduct<T1, TN...>();
}

/** Applies fn to one element (SEQ) of the cartesian product of
 * the argument arrays. */
template<int SEQ, typename FN, typename T1, typename... TN>
void for_seq(FN fn, T1 && t1, TN &&... tn) {
	using namespace Detail;
	ForSeq<SEQ, 1 + sizeof...(TN), FN, T1, TN...>
			::apply(fn, FWD(T1,t1), FWD(TN,tn)...);
}

/** Applies fn to each element of the cartesian product of the
 * argument arrays. */
template<typename FN, typename T1, typename... TN,
	int N = Detail::Sizes<T1, TN..., Detail::delim_tag>::SIZE>
void for_all(FN fn, T1 && t1, TN &&... tn) {
	using namespace Detail;
	ForAll<N, FN, T1, TN...>::apply(fn,
			FWD(T1,t1), FWD(TN,tn)...);
}

template<typename FN0, typename FN1, typename... FN, typename T1,
	typename... TN, int N = Detail::Sizes<T1, TN..., Detail::delim_tag>::SIZE>
void for_all(FN0 fn0, FN1 fn1, FN... fn, T1 && t1, TN &&... tn) {
	for_all(fn0, SELF(t1), SELF(tn)...);
	for_all(SELF(fn1), SELF(fn)..., SELF(t1), SELF(tn)...);
}

/** Applies fn to each element of the cartesian product of the
 * argument arrays and stores the results sequentially in out. */
template<typename FN, typename T1, typename... TN,
	typename RET = typename Detail::Signature<FN>::result_type,
	int N = Detail::Sizes<T1, TN..., Detail::delim_tag>()>
auto transform_for_all(FN fn, RET (&out)[N],
		T1 && t1, TN &&... tn) -> decltype(out) {
	using namespace Detail;
	TransformForAll<N, FN, T1, TN...>::apply(fn,
			FWD(RET (&)[N],out), FWD(T1,t1), FWD(TN,tn)...);
	return FWD(RET (&)[N],out);
}

template<typename FN, int N, typename T1, typename... TN>
void for_zip(FN fn, T1 (&t1)[N], TN (&...tn)[N]) {
	using namespace Detail;
	ForZip<N, N, FN, T1, TN...>::apply(fn,
			FWD(T1(&)[N],t1), FWD(TN(&)[N],tn)...);
}

namespace test {
struct test_util {
	static bool run(void) {
		using namespace Detail;
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

		// Types with duplicates
		typedef pack_t<int,int> T_II;
		typedef pack_t<float, double, float> T_FDF;
		
		
		// Concatenation types
		static_assert(std::is_same<typename
				pack_cat<T_I, float>::type, T_IF>::value, "");
		static_assert(std::is_same<typename
				pack_cat<T_I, int>::type, T_I>::value, "");

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
		static_assert(std::is_same<
				decltype(indices_of(VIFD, VD + VI + pack_t<char>{})),
				pack_i<2, 0, -1>>::value, "");

		// Prune duplicates
		static_assert(std::is_same<decltype(prune(VIFD)), T_IFD>::value, "");
		static_assert(std::is_same<decltype(prune(T_FDF {})),
				decltype(VF+VD)>::value, "");
		
		// Or
		static_assert(std::is_same<decltype(VI+VF+VD), T_IFD>::value, "");
		static_assert(!std::is_same<decltype(VD+VF+VI), T_IFD>::value, "");
		// Not
		static_assert(std::is_same<decltype(VIF-VF), T_I>::value, "");
		static_assert(std::is_same<decltype(VIF-VF-VI), T_void>::value, "");
		// Xor
		static_assert(std::is_same<decltype(VI^VI),
				decltype(VI-VI)>::value, "");
		static_assert(std::is_same<decltype(VI^VF),
				decltype(VI+VF)>::value, "");
		// And
		static_assert(std::is_same<decltype(VI & VI), T_I>::value, "");
		static_assert(std::is_same<decltype(VIF & VI), T_I>::value, "");

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
		static_assert(!inner_value((packed1 <same_as> VI)), "");
		static_assert(inner_value((prune(packed1) <same_as> VI)), "");

		auto packed2 = 3 <pack_with> 4.0f;
		static_assert(inner_value(packed2 <same_as> VIF), "");
	
		graph<T_void, T_void> G_void;
		auto G_if = G_void + node<int>{} + node<float>{};
		static_assert(inner_value(G_if <same_as> G_if), "");
		return true;
	}
};

}

#endif
