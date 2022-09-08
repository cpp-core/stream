// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <tuple>
#include "coro/stream/util.h"
#include "coro/stream/sampler/integral.h"
#include "core/tuple/apply.h"
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


/// Return a **Stream** that randomly chooses elements from the **Stream**s in `tup` until
/// all **Stream** are exhausted.
template<Stream S, Stream... Ss>
Generator<streams_yield_t<S, Ss...>> choose(std::tuple<S, Ss...> tup) {
    static_assert(sizeof...(Ss) < 5);
    auto r = sampler<int>(0, sizeof...(Ss));
    using namespace core::tp;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto sentinels = mapply([](auto& g) { return g.end(); }, tup);
    while (any(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, sentinels))) {
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

/// Choose values randomly from a tuple of **Stream**s.
///
/// Return a function that accepts a tuple of **Stream**s and randomly chooses elements
/// from the streams unitl all **Stream**s are exhausted.
///
/// *sampler<int>(0, 9) x sampler<int>(10, 19) x sampler<int>(20, 29) | choose()*
inline auto choose() {
    return []<class T>(T tup) {
	return choose(std::move(tup));
    };
}

}; // coro
