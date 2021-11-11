// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields the elements from the supplied
// `generator` transformed by the given function `func` that maps
// **T** to **U**.
template<class T, class F, class U = std::result_of_t<F&(T)>>
Generator<U> transform(Generator<T> generator, F&& func) {
    for (auto elem : generator)
	co_yield func(elem);
    co_return;
}

// Return a function that accepts generator **G** and returns a new
// `transform` generator. The `transform` generator yields the
// elements yielded from **G** transformed by `func`.
template<class F>
auto transform(F func) {
    return [=]<class G>(G&& g) {
	return transform(std::move(g), func);
    };
}

}; // coro
