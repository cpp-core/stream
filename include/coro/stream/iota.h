// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

/// Return a generator that yields `count` number of **T**'s starting
/// with `start` and incrementing by `step`.
template<class T>
Generator<const T&> iota(size_t count, T start = T{0}, T step = T{1}) {
    for (auto i = 0; i < count; ++i, start += step)
	co_yield start;
    co_return;
}

}; // coro
