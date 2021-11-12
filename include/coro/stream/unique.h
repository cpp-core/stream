// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields the elements from the supplied `generator` filtering
// them to be unique with respect to the given `key` function.
template<class T, class F>
Generator<T> unique(Generator<T> generator, F&& key) {
    using K = std::result_of_t<F(T)>;
    std::set<K> s;
    for (auto element : generator) {
	auto k = key(element);
	if (not s.contains(k)) {
	    co_yield element;
	    s.insert(k);
	}
    }
    co_return;
}

// Filter elements from a generator to make them unique with respect to the given `key`
// function which defaults to identity.
//
// Return a function that accepts a generator **G** and returns a new
// `unique` generator. The `unique` generator yields the elements of
// **G** filtered with respect to the given `key` function.
//
// *sampler<int>(0, 100) | unique([](int n) { return n % 11; })*
template<class F>
auto unique(F key = [](const auto& elem) { return elem; }) {
    return [=]<class G>(G&& g) {
	return unique(std::forward<G>(g), key);
    };
}

}; // coro
