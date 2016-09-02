#ifndef UTIL_HPP
#define UTIL_HPP

#include <functional>
#include <atomic>

#ifndef FWD
#define FWD(X,Y) std::forward<X>(Y)
#endif

#ifndef SELF
#define SELF(Y) std::forward<decltype(Y)>(Y)
#endif

namespace Util { // --> util/types.hpp

	/// A type representing no type
	struct undef_t {}; 
	/// A type delimiting a type pack
	struct delim_t {}; 

	/*! A type that allows a binary template to be infixed
	 * @tparam C The binary template type
	 * @tparam T Template arguments into C
	 */
	template<template<class... TN> class C, class... T>
	struct infix_t;

	/*! A type used to find the static dimension of a type
	 * @tparam T A statically-sized type
	 */
	template<class T> struct sized_t;

	/*! Reduces a variadic list of types to the sum of their sizes
	 * @tparam T1 The head of the argument types
	 * @tparam TN The remainder of the template arguments
	 */
	template<class T1, class... TN> struct sizes_t;

	/*! Assigns each instance a unique sequential ID
	 * @tparam T The type of the instances to count
	 */
	template<typename T> struct counted_t;

	/*! A tag type storing types with duplicates
	 * @tparam T The unordered list of types
	 */
	template<class... T> struct pack_t;

	/*! A tag type storing integers with duplicates
	 * @tparam I The unordered list of integers
	 */
	template<int... I> struct pack_i;

	/*! A tag type storing types without duplicates
	 * @tparam T The unordered list of unique types
	 */
	template<class... T> struct set_t;

	/*! A tag representing a node in a graph
	 * @tparam T The type corresponding to the node
	 */
	template<class T> struct node_t;

	/*! A tag representing an edge between two nodes in a graph
	 * @tparam U The ID of the source of the edge, or both
	 * @tparam V The ID of the destination of the edge, or both
	 */
	template<int U, int V> struct edge_t;

	/*! A tag representing a graph
	 * @tparam V The pack of types containing the vertex endpoints
	 * @tparam E The pack of integer pairs containing the endpoint indices
	 * @tparam BIDI True if and only if edges are bidirectional
	 */
	template<class V, class E, bool BIDI=false> struct graph_t;

	/*! A type used to select a member of a type pack
	 * @tparam T The pack of types
	 * @tparam I The index into the pack
	 */
	template<class T, int I> struct pack_get_t;

	/*! A structure that merges two type packs without introducing
	 * duplicates; does not prevent duplicates in general
	 * @tparam U The left operand, an unordered pack of types
	 * @tparam V The right operand, an unordered pack of types
	 */
	template<typename U, typename V> struct pack_merge;

	/*! A structure that removes the types in one pack from another
	 * @tparam U The subtrahend pack of types
	 * @tparam V The minuend pack of types
	 * @tparam W The partial difference of the preceding packs
	 */
	template<typename U, typename V, typename W> struct pack_remove;
}
#include "util/types.hpp"

namespace Util { // --> util/functional.hpp
	/*! A tag representing the signature of a function
	 * @tparam S A function or function object type
	 */
	template<class S>
		struct sig_t;

	/*! A type exposing an application function over a permutation
	 * of elements of array types
	 * @tparam SEQ The unique ID of the permutation
	 * @tparam CUR The index into the permutation
	 * @tparam TN The unordered list of array types
	 */
	template<int SEQ, int CUR, class... TN>
		struct for_seq_t;

	/*! A type exposing an application function over all permutations
	 * of elements of array types 
	 * @tparam SEQ The unique ID of the current permutation
	 * @tparam FN The type of the function or function object to apply
	 * @tparam TN The unordered list of array types
	 */
	template<int SEQ, class FN, class... TN>
		struct for_all_t;

	/*! Maps each array to another type before applying a function to
	 * each permutation of the resulting elements.
	 * @tparam SEQ The unique ID of the current sequence
	 * @tparam FN The function to apply to each permutation
	 * @tparam T1 The head of the array types
	 * @tparam TN The remainder of the array types
	 */
	template<int SEQ, class FN, class T1, class... TN>
		struct map_for_all_t;

	/*! Applies a function to parallel sets of elements of the given
 	 * statically-sized arrays
	 * @tparam SEQ The unique ID of the current sequence
	 * @tparam FN The function to apply to each selected permutation
	 * @tparam N The size of each array type
	 * @tparam T1 The head of the list of array types
	 * @tparam TN The remainder of the array types
	 */
	template<int SEQ, int N, class FN, class T1, class... TN>
		struct for_zip_t;
}
#include "util/functional.hpp"
#include "util/task.hpp"

/** Applies the given function to each pair of elements
 * @tparam S The expected type of the arguments
 * @param fn The function to apply to the arguments
 */
template<typename... S>
void zipper(std::function<void(S...)> fn) {}

/** Applies the given function to each pair of elements
 * @tparam S1 The type of the first element from the first pack
 * @tparam SN The type of the remaining elements of the second pack
 * @tparam T1 The type of the first element of the second pack
 * @tparam TN The type of the remaining elements of the first pack
 * @tparam FN The type of the function to apply to the given elements
 * @param fn The function to apply to the given elements
 * @param s1 The first element from the first pack
 * @param sn The remaining elements from the first pack
 * @param t1 The first element of the second pack
 * @param tn The remaining elements from the first pack
 */
template<typename S1, typename... SN, typename T1, typename... TN,
	template<typename> class FN = std::function>
void zipper(FN<void(S1, SN...)> fn,
		S1 && s1, SN &&... sn,
		T1 && t1, TN &&... tn) {
	fn(SELF(s1), SELF(sn)...);
	zipper(fn, SELF(t1), SELF(tn)...);
}

/** Abstraction of \ref zipper to arrays of elements
 * @tparam FN The type of the function to apply
 * @tparam N The number of elements in all arrays
 * @tparam T1 The type of the first argument array
 * @tparam TN The type of the rest of the argument arrays
 * @param fn The function to apply
 * @param t1 The first argument array
 * @param tn The remainder of the argument arrays
 */
template<typename FN, int N, typename T1, typename... TN>
void for_zip(FN fn, T1 (&t1)[N], TN (&... tn)[N]) {
	using namespace Util;
	for_zip_t<N, N, FN, T1, TN...>::apply(fn,
			FWD(T1(&)[N],t1), FWD(TN(&)[N],tn)...);
}

/** Calculates the product of sizes for statically-sized members
 * @tparam T1 The first statically-sized template argument
 * @tparam TN The remainder of the template arguments
 * @return The product of all argument sizes
 */
template<typename T1, typename... TN>
constexpr int SizeProduct(void) {
	return Util::sizes_t<T1, TN..., Util::delim_t>::SIZE;
}

/** Calculates the product of sizes for statically-sized members
 * @tparam T1 The first statically-sized template argument
 * @tparam TN The remainder of the template arguments
 * @return The product of all argument sizes
 */
template<typename T1, typename... TN>
constexpr int SizeProduct(T1 && t1, TN &&... tn) {
	return SizeProduct<T1, TN...>();
}

/**! Applies fn to one element (SEQ) of the cartesian product of
 * the argument arrays. */
template<int SEQ, typename FN, typename T1, typename... TN>
void for_seq(FN fn, T1 && t1, TN &&... tn) {
	using namespace Util;
	for_seq_t<SEQ, 1 + sizeof...(TN), FN, T1, TN...>
			::apply(fn, FWD(T1,t1), FWD(TN,tn)...);
}

/**! Applies fn to each element of the cartesian product of the
 * argument arrays.
 * @tparam FN The type of the function to apply
 * @tparam T1 The type of the first argument
 * @tparam TN the type of the remainder of the arguments
 * @tparam N The product of the argument sizes
 * @param fn The function to apply
 * @param t1 The first argument
 * @param tn The remainder of the arguments
 */
template<typename FN, typename T1, typename... TN,
	int N = Util::sizes_t<T1, TN..., Util::delim_t>::SIZE>
void for_all(FN fn, T1 && t1, TN &&... tn) {
	using namespace Util;
	for_all_t<N, FN, T1, TN...>::apply(fn,
			FWD(T1,t1), FWD(TN,tn)...);
}
template<typename FN0, typename FN1, typename... FN, typename T1,
	typename... TN, int N = Util::sizes_t<T1, TN..., Util::delim_t>::SIZE>
void for_all(FN0 fn0, FN1 fn1, FN... fn, T1 && t1, TN &&... tn) {
	for_all(fn0, SELF(t1), SELF(tn)...);
	for_all(SELF(fn1), SELF(fn)..., SELF(t1), SELF(tn)...);
}

/** Applies fn to each element of the cartesian product of the
 * argument arrays and stores the results sequentially in out. */
template<typename FN, typename T1, typename... TN,
	typename RET = typename Util::sig_t<FN>::result_type,
	int N = Util::sizes_t<T1, TN..., Util::delim_t>()>
auto map_for_all(FN fn, RET (&out)[N],
		T1 && t1, TN &&... tn) -> decltype(out) {
	using namespace Util;
	map_for_all_t<N, FN, T1, TN...>::apply(fn,
			FWD(RET (&)[N],out), FWD(T1,t1), FWD(TN,tn)...);
	return FWD(RET (&)[N],out);
}

#endif
