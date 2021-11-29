// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields `value` exactly `count` times.
template<class T>
Generator<const T&> repeat(T value, size_t count = std::numeric_limits<size_t>::max()) {
    while (count--)
	co_yield value;
    co_return;
}

}; // coro
