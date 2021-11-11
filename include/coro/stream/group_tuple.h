// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/tuple/from_vector.h"
#include "core/tuple/generate.h"

namespace coro {

// Return a generator that yields **std::tuple<`T`,..>**'s with arity
// `N`. The generator groups elements yielded from the supplied
// `generator` into `N`-tuples and yields them.
template<size_t N, class T>
Generator<core::tp::generate_t<T, N>> group_tuple(Generator<T> generator) {
    std::vector<T> data;
    while (generator.next()) {
	data.push_back(generator());
	if (data.size() == N) {
	    co_yield core::tp::from_vector<T,N>(data);
	    data.clear();
	}
    }
    co_return;
}

// Return a function that accepts generator **G** and returns a new
// `group_tuple` generator. The `group_tuple` generator yields
// **std::tuple<`G::value_type`,...>**'s constructed by grouping
// elements yielded from **G**.
//
// *sampler<int>(0, 100) | group_tuple<2>() -> std::tuple<int,int>*
template<size_t N>
auto group_tuple() {
    return [=]<class G>(G&& g) {
	return group_tuple<N>(std::move(g));
    };
}

}; // coro
