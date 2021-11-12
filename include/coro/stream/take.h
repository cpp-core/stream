// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields the first `count` elements (or until
// exhaustion) from the supplied `generator`.
template<class T>
Generator<T> take(Generator<T> generator, size_t count) {
    while (count-- > 0 and generator.next())
	co_yield generator();
    co_return;
}

// Take the first `count` elements from the preceeding generator.
//
// Usage: *sampler<int>() | take(10)*
inline auto take(size_t count) {
    return [=]<class G>(G&& g) {
	return take(std::move(g), count);
    };
}

}; // coro
