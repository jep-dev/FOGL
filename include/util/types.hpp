#ifndef UTIL_TYPES_HPP
#define UTIL_TYPES_HPP

#include "util.hpp"

namespace Util {

	template<typename T, int N>
	struct sized_t<T (&) [N]> {
		static constexpr int SIZE = N;
	};
	template<template<typename...> class C, typename... T>
	struct sized_t<C<T...>> {
		static constexpr int SIZE = C<T...>::size;
	};
	template<typename T>
	struct sized_t {
		static constexpr int SIZE = 1;
	};

	template<typename T1, typename... TN>
	struct sizes_t {
		static constexpr int SIZE = sized_t<T1>::SIZE
			* sizes_t<TN...>::SIZE;
	};
	template<typename... TN>
	struct sizes_t<delim_t, TN...> {
		static constexpr int SIZE = 1;
	};

	template<typename T> struct counted_t {
		const int instance_id;
		counted_t(void): instance_id(next_id()) {}
		static int peek(void) {return next_id(true);}
	private:
		static int next_id(bool peek=false) {
			static int ctr = 0;
			return peek ? ctr : ctr++;
		}
	};

	template<typename... T>
	struct pack_t {};

	template<int... N>
	struct pack_i {};

	template<typename... T>
	struct set_t:
		public decltype(prune(pack_t<T...>{})) {};

	template<typename T> constexpr typename T::type
	inner_type (const T) { return {}; }

	template<typename T>
	constexpr auto inner_value (const T)
	-> decltype(T::value) { return T::value; }

	template<typename T, int I = 0>
	constexpr int index_of (pack_t<>, T t,
			pack_i<I> = pack_i<I>{}) {
		return -1;
	}
	template<typename S1, typename... SN,
		typename T, int I = 0>
	constexpr int index_of (pack_t<S1, SN...>, T t,
			pack_i<I> = pack_i<I>{}) {
		return std::is_same<T,S1>::value ? I :
			index_of(pack_t<SN...> {}, t, pack_i<I+1> {});
	}
	template<typename... S, typename... T>
	constexpr auto indices_of(pack_t<S...> u, pack_t<T...> v)
	-> pack_i<index_of(u, T {})...> {
		return {};
	}

	template<int I1, int I2, int... IN>
	constexpr int sum_of(pack_i<I1, I2, IN...>) {
		return I1 + I2 + sum_of(pack_i<IN...> {});
	}
	template<int I1, int... IN>
	constexpr int sum_of(pack_i<I1, IN...>) {
		return I1 + sum_of(pack_i<IN...> {});
	}
	constexpr int sum_of(pack_i<>) {
		return 0;
	}
	template<typename T1, typename... TN>
	constexpr auto rotate(pack_t<T1, TN...>)
	-> pack_t<TN..., T1> {
		return {};
	}
	
	template<typename... PREV>
	struct pack_merge<pack_t<PREV...>, pack_t<>> {
		typedef pack_t<PREV...> type;
	};
	template<typename... PREV, typename CUR, typename... NEXT>
	struct pack_merge<pack_t<PREV...>, pack_t<CUR, NEXT...>> {
		typedef typename std::conditional<
					index_of(pack_t<PREV...>{}, CUR{}) >= 0,
					pack_t<PREV...>, pack_t<PREV..., CUR>
				>::type cond_type;
		typedef typename pack_merge<cond_type, pack_t<NEXT...>>::type type;
	};

	template<typename PRE, typename DEL, typename POST>
	struct pack_remove;
	template<typename... PRE, typename... POST>
	struct pack_remove<pack_t<PRE...>, pack_t<>, pack_t<POST...>> {
		typedef pack_t<PRE..., POST...> type;
	};
	template<typename DEL_1, typename... DEL_N, typename... POST>
	struct pack_remove<pack_t<>, pack_t<DEL_1, DEL_N...>, pack_t<POST...>> {
		typedef typename pack_remove<pack_t<POST...>,
				pack_t<DEL_N...>, pack_t<>>::type type;
	};
	template<typename PRE_1, typename... PRE_N,
		typename DEL_1, typename... DEL_N, typename... POST>
	struct pack_remove<pack_t<PRE_1, PRE_N...>,
			pack_t<DEL_1, DEL_N...>, pack_t<POST...>> {
		typedef typename std::conditional<std::is_same<DEL_1, PRE_1>::value,
				pack_t<POST...>, pack_t<PRE_1, POST...>>::type rtype;
		typedef typename pack_remove<pack_t<PRE_N...>,
				pack_t<DEL_1, DEL_N...>, rtype>::type type;
	};

	template<typename... A, typename... B, typename C = 
		typename pack_merge<pack_t<A...>, pack_t<B...>>::type>
	constexpr C operator+(pack_t<A...>, pack_t<B...>) {return C {};}

	template<typename... A, typename... B, typename C = typename
		pack_remove<pack_t<A...>, pack_t<B...>, pack_t<>>::type>
	constexpr C operator-(pack_t<A...>, pack_t<B...>) {return C {};}

	template<typename... A, typename... B,
		typename C = pack_t<A...>, typename D = pack_t<B...>,
		typename E = decltype((C{}-D{})+(D{}-C{}))>
	constexpr E operator^(C, D) {return E {};}

	template<typename... A, typename... B,
		typename C = pack_t<A...>, typename D = pack_t<B...>,
		typename E = decltype((C{}+D{})-(C{}^D{}))>
	constexpr E operator&(C, D) {return E {};}


	template<typename... A, typename B = typename
		pack_merge<pack_t<>, pack_t<A...>>::type>
	constexpr B prune(pack_t<A...>) {return B {};}

	template<typename... A, typename... B,
		typename C = pack_t<A...>, typename D = pack_t<B...>>
	constexpr bool permutes(C c, D d) {
		// TODO -- distinguish between perm. w/wo duplicates
		return std::is_same<decltype(c^d), pack_t<>>::value;
	}

	template<typename S, typename T1, typename... TN>
	constexpr bool contains(pack_t<T1, TN...>, S && s) {
		return contains(pack_t<TN...>{}, FWD(S,s));
	}
	template<typename S, typename... TN>
	constexpr bool contains(pack_t<S, TN...>, S) {
		return true;
	}
	template<typename S>
	constexpr bool contains(pack_t<>, S) {
		return false;
	}

	template<int U, int V>
	struct edge_t: pack_i<U,V> {};
	template<typename T>
	struct node_t: pack_t<T> {};

	template<typename... V, typename... E, bool BIDI>
	struct graph_t<pack_t<V...>, pack_t<E...>, BIDI> {
		typedef pack_t<V...> vertices;
		typedef pack_t<E...> edges;

		template<typename T>
		constexpr graph_t<decltype(vertices {}
				+ node_t<T> {}), edges>
		operator+(node_t<T>) const {
			return {};
		}
		template<int S, int D>
		constexpr graph_t<vertices, decltype(
				prune(edges {} + pack_t<edge_t<S, D>> {}))>
		operator+(edge_t<S, D>) const {
			return {};
		}
	};

	template<typename T1, typename... TN, int I>
	struct pack_get_t<pack_t<T1, TN...>, I> {
		typedef typename std::conditional<I==0, T1,
				typename pack_get_t<pack_t<TN...>, I-1>::type
			>::type type;
	};

	template<template<typename...> class C,
		typename A, typename B>
	struct infix_t {typedef C<A, B> type;};
	
	template<template<typename...> class C, typename A, typename B>
	constexpr C<A,B> operator>(infix_t<C, A>, B) {return {};}
	
	template<template<typename...> class C, typename A>
	constexpr infix_t<C, A> operator<(A, infix_t<C>) {return {};}


}

#endif
