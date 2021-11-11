// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Yield either the first `count` or until generator exhaustion
// values of type `T` from the supplied `generator`.
template<class T>
Generator<T> take(coro::Generator<T> generator, size_t count) {
    while (count-- > 0 and generator.next())
	co_yield generator();
    co_return;
}

// Return a unary take operator with a fixed `count`.
auto take(size_t count) {
    return [=]<class G>(G&& g) {
	return take(std::move(g), count);
    };
}

}; // costr
