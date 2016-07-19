#ifndef UTIL_FUNCTION_HPP
#define UTIL_FUNCTION_HPP

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

#endif
