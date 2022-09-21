// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <tuple>
#include "coro/stream/util.h"
#include "coro/stream/sampler/integral.h"
#include "core/tuple/fold.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace coro {

#define APPLY_NTH(Idx)						\
    if constexpr (std::tuple_size<decltype(tup)>() > Idx) {	\
	if (idx == Idx) {					\
	    auto& iter = std::get<Idx>(iterators);		\
	    auto& end = std::get<Idx>(sentinels);		\
	    if (iter != end) {					\
		co_yield *iter;					\
		++iter;						\
	    }							\
	}							\
    }


/// Choose elements uniformly from the given `tuple` of Streams unitl
/// all are exhausted.
///
/// Returns a **Generator** that yields yields elements from the input
/// Streams choosen at random until all are exhausted.
///
/// \tparam S A source that satisfies the `Stream` concept.
/// \tparam Ss A source(s) that satisfies the `Stream` concept.
template<Stream S, Stream... Ss>
Generator<streams_yield_t<S, Ss...>> choose(std::tuple<S, Ss...> tup) {
    static_assert(sizeof...(Ss) < 5);
    auto r = sampler<int>(0, sizeof...(Ss));
    using namespace core::tp;
    auto iterators = map_inplace([](auto& g) { return g.begin(); }, tup);
    auto sentinels = map_inplace([](auto& g) { return g.end(); }, tup);
    while (any(map([](auto& iter, auto& end) { return iter != end; }, iterators, sentinels))) {
	auto idx = r.sample();
	APPLY_NTH(0);
	APPLY_NTH(1);
	APPLY_NTH(2);
	APPLY_NTH(3);
	APPLY_NTH(4);
    }
    co_return;
}

#undef APPLY_NTH

/// Choose elements uniformly from a tuple of Streams unitl all are exhausted.
///
/// Returns a function that accepts a tuple of input Stream's and
/// returns a new Stream that yields elements from the input Streams
/// choosen at random until all are exhausted.
///
/// \rst
/// ```{code-block} c++
/// sampler<int>(0, 9) * sampler<int>(10, 19) * sampler<int>(20, 29) | choose();
/// // maybe 13, 4, 25, 11, 18, 4, 6, 24...
/// ```
/// \endrst
inline auto choose() {
    return []<class T>(T tup) {
	return choose(std::move(tup));
    };
}

}; // coro
