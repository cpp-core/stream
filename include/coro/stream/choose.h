// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields elements from the underlying generators in a uniform
// random fashion.
template<class T, size_t N>
Generator<T> choose(std::array<Generator<T>,N> arr) {
    auto gidx = sampler<int>(0, arr.size() - 1);
    size_t exhausted{0};
    while (exhausted < arr.size()) {
	auto idx = gidx.sample();
	while (arr[idx].done())
	    idx = (idx + 1) % arr.size();
	if (arr[idx].next())
	    co_yield arr[idx]();
	else
	    ++exhausted;
    }
    co_return;
}

// Choose randomly from an array of generators for each element.
//
// *Returns:* **Generator<...>** A generator that yields elements from the underlying
// generators in a uniform random fashion.
//
// *sampler<int>(0, 9) + sampler<int>(10, 19) + sampler<int>(20, 29) | choose()*
inline auto choose() {
    return []<class G>(G g) {
	return alternate(std::move(g));
    };
}

}; // coro
