// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a **Generator<`T`>** that yields the elements from the
// supplied `generator` filtered by the given `predicate`.
template<class T, class P>
Generator<T> filter(Generator<T> generator, P&& predicate) {
    for (auto element : generator)
	if (predicate(element))
	    co_yield element;
    co_return;
}

// Return a function that accepts a generator **G** and returns a
// generator that filters **G** using the supplied `predicate`.
//
// *sampler<int>(0, 100) | filter([](int n) { return n % 2 == 0; })*
template<class P>
auto filter(P predicate) {
    return [=]<class G>(G&& g) {
	return filter(std::forward<G>(g), predicate);
    };
}

}; // costr
