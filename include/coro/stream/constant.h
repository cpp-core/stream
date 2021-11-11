// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a generator that yields `value` indefinitely.
template<class T>
Generator<T> constant(T value) {
    while (true)
	co_yield value;
    co_return;
}

}; // coro
