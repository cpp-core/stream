// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

/// Return a generator that yields the values from `start` until `stop`
/// incrementing by `step`.
template<class T>
Generator<const T&> range(T start, T stop, T step = T{1}) {
    for (auto x = start; x <= stop; x += step)
	co_yield x;
    co_return;
}

}; // coro
