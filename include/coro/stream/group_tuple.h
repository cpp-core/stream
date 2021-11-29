// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/tuple/from_vector.h"
#include "core/tuple/generate.h"

namespace coro {

// Return a generator that yields **std::tuple<`T`,..>**'s with arity
// `N`. The generator groups elements yielded from the supplied
// `generator` into `N`-tuples and yields them.
template<size_t N, Stream S>
Generator<core::tp::generate_t<stream_value_t<S>, N>&&> group_tuple(S source) {
    using T = stream_value_t<S>;
    std::vector<T> data;
    for (auto&& elem : source) {
	data.push_back(elem);
	if (data.size() == N) {
	    co_yield core::tp::from_vector<T,N>(data);
	    data.clear();
	}
    }
    co_return;
}

// Group the elemens of the preceeding generator into N-tuples.
//
// *Returns:* **Generator<std::tuple<...>>** A generator that yields **std::tuple<...>**'s
// with arity `N`. The generator groups elements yielded from the underlying generator
// into `N`-tuples and yields them.
//
// *sampler<int>(0, 100) | group_tuple<2>() // -> std::tuple<int,int>*
template<size_t N>
auto group_tuple() {
    return [=]<class G>(G&& g) {
	return group_tuple<N>(std::move(g));
    };
}

}; // coro
