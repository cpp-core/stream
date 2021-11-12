// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields elements from the underlying generators starting with
// all the elements from the first generator before proceeding to the next generator.
template<class T, size_t N>
Generator<T> sequence(std::array<Generator<T>,N> arr) {
    for (auto i = 0; i < arr.size(); ++i)
	for (auto elem : arr[i])
	    co_yield elem;
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
