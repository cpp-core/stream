// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

/// Return a generator that yields `value` exactly once.
template<class T>
Generator<T&&> once(T value) {
    co_yield value;
    co_return;
}

}; // coro
