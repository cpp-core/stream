// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/sampler/integral.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace coro {

// Return a **Stream** that randomly chooses elements from the **Stream**s in `tup` until
// all **Stream** are exhausted.
template<Stream S, Stream... Ss>
Generator<stream_value_t<S>&&> choose(std::tuple<S, Ss...> tup) {
    auto r = sampler<int>(0, sizeof...(Ss));
    using namespace core::tp;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    while (any(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	while (true) {
	    auto idx = r.sample();
	    auto& iter = select_nth(iterators, idx);
	    auto& end = select_nth(end_iters, idx);
	    if (iter != end) {
		co_yield *iter;
		++iter;
		break;
	    }
	}
    }
    co_return;
}

// Choose values randomly from a tuple of **Stream**s.
//
// Return a function that accepts a tuple of **Stream**s and randomly chooses elements
// from the streams unitl all **Stream**s are exhausted.
//
// *sampler<int>(0, 9) x sampler<int>(10, 19) x sampler<int>(20, 29) | choose()*
inline auto choose() {
    return []<class T>(T tup) {
	return choose(std::move(tup));
    };
}

}; // coro
