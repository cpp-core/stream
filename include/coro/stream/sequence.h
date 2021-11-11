// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/mp/foreach.h"
#include "coro/stream/util.h"

namespace coro {

// Compose generators **G** and **H** into an array tuple. This facilitates integrating
// multiple generators into a chain.
//
// *sampler<int>(0, 10) ^ sampler<int>(0, 5) | zip() | take(5)* 
template<class G, class H>
requires (is_generator_v<G> && is_generator_v<H>)
auto operator&(G&& g, H&& h) {
    using T = typename std::decay_t<G>::value_type;
    return std::array<Generator<T>,2>{std::forward<G>(g), std::forward<H>(h)};
}

template<class Op, class G, size_t N>
requires is_generator_v<G>
auto operator|(std::array<G,N>&& arr, Op&& op) {
    return op(std::forward<std::array<G,N>>(arr));
}

// Return a generator that yields the values from the underlying
// generators in sequence, i.e. all the values from the first
// generators, then all the values from the second generator, etc.
template<class T, size_t N>
Generator<T> sequence(std::array<Generator<T>,N>&& arr) {
    for (auto i = 0; i < arr.size(); ++i)
	for (auto elem : arr[i])
	    co_yield elem;
    co_return;
}

inline auto sequence() {
    return []<class G>(G&& g) {
	return sequence(std::forward<G>(g));
    };
}

}; // coro
