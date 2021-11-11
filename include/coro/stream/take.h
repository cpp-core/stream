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

// Return a function that accepts generator **G** and returns a new
// `take` generator. The `take` generator yields the first `count`
// elements (or until exhaustion) from **G**.
auto take(size_t count) {
    return [=]<class G>(G&& g) {
	return take(std::move(g), count);
    };
}

}; // coro
