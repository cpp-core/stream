// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields the elements from the supplied `generator` transformed
// by the given function `func` that maps **T** to **U**.
template<class T, class F, class U = std::result_of_t<F&(T)>>
Generator<U> transform(Generator<T> generator, F&& func) {
    for (auto elem : generator)
	co_yield func(elem);
    co_return;
}

// Transform the elements from the preceeding generator using the supplied `func`.
// 
// *Returns:* **Generator<...>** A generator that yields the values from the underlying
// *generator transformed by `func`.
//
// *sampler<int>(0, 10) | transform([](int n) { return n + 10; })*
template<class F>
auto transform(F func) {
    return [=]<class G>(G&& g) {
	return transform(std::move(g), func);
    };
}

}; // coro
