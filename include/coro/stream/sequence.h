// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/tuple/map.h"

namespace coro {

// Return a generator that yields elements from the underlying generators starting with
// all the elements from the first generator before proceeding to the next generator.
template<Stream S, Stream... Ss>
Generator<stream_value_t<S>&&> sequence(std::tuple<S, Ss...> tup) {
    using core::tp::select_nth, core::tp::mapply;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    for (auto i = 0; i < sizeof...(Ss) + 1; ++i) {
	auto& iter = select_nth(iterators, i);
	auto& end = select_nth(end_iters, i);
	while (iter != end) {
	    co_yield *iter;
	    ++iter;
	}
    }
    co_return;
}

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
