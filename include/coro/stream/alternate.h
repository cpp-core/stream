// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace coro {

// Return a **Stream** that returns elements in a round-robin fashion from the given tuple
// of **Stream**'s `tup` until all are exhausted.
template<Stream S, Stream... Ss>
Generator<const stream_value_t<S>&> alternate(std::tuple<S, Ss...> tup) {
    using namespace core::tp;
    using value_type = stream_value_t<S>;
    
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    while (any(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	auto values = map_n([](auto& iter, auto& end) {
	    if (iter != end) return std::optional<value_type>{*iter};
	    else return std::optional<value_type>{};
	}, iterators, end_iters);
	auto arr = to_array(values);
	for (auto i = 0; i < std::tuple_size<decltype(values)>(); ++i)
	    if (arr[i])
		co_yield arr[i].value();
	apply_n([](auto& iter, auto& end) { if (iter != end) ++iter; }, iterators, end_iters);
    }
    co_return;
}

// Alternate elements from a tuple of **Stream**'s until all are exhausted.
//
// Return a function that accepts a tuple of **Stream**'s and returns a new **Stream**
// that returns elements in a round-robin fashion from the underlying **Stream**'s until
// all are exhausted
//
// *sampler<int>(0, 9) x sampler<int>(10, 19) x sampler<int>(20, 29) | alternate()*
inline auto alternate() {
    return []<class T>(T&& source) {
	return alternate(std::move(source));
    };
}

}; // coro
