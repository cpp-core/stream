// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/tuple/map.h"

namespace coro {

#define APPLY_NTH(Idx)				       \
    if constexpr (sizeof...(Ss) > Idx) {	       \
	auto& iter = std::get<Idx>(iterators);	       \
	auto& end = std::get<Idx>(end_iters);	       \
	while (iter != end) {			       \
	    co_yield *iter;			       \
	    ++iter;				       \
	}					       \
    }

// Return a generator that yields elements from the underlying generators starting with
// all the elements from the first generator before proceeding to the next generator.
template<Stream... Ss>
requires (sizeof...(Ss) > 0)
Generator<streams_yield_t<Ss...>> sequence(std::tuple<Ss...> tup) {
    using core::tp::select_nth, core::tp::mapply;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    APPLY_NTH(0);
    APPLY_NTH(1);
    APPLY_NTH(2);
    APPLY_NTH(3);
    APPLY_NTH(4);
    co_return;
}

#undef APPLY_NTH

// Sequence the elements from the preceeding array of generators.
//
// *Returns:* **Generator<...>** A generator that yields elements from the underlying
// generators starting with all the elements from the first generator before proceeding to
// the next generator.
//
// *sampler<int>(0, 9) + sampler<int>(10, 19) + sampler<int>(20, 29) | sequence()*
inline auto sequence() {
    return []<class G>(G g) {
	return sequence(std::move(g));
    };
}

}; // coro
