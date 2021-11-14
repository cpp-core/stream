// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Apply `func` to each element yielded from the supplied `generator` and return the
// number of elements processed.
template<class T, class F>
size_t apply(Generator<T> generator, F&& func) {
    size_t count{0};
    for (auto&& value : generator) {
	std::forward<F>(func)(value);
	++count;
    }
    return count;
}

// Apply `func` to each element from a generator and return the number of elements
// processed.
//
// Return a function that accepts **Generator<`T`>** and returns a **size_t** indicated
// the number of elements yieled from the generator.
template<class F>
auto apply(F func) {
    return [=]<class T>(Generator<T>&& g) {
	return apply(std::forward<Generator<T>>(g), std::move(func));
    };
}

}; // coro
