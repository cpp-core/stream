// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace costr {

template<class T>
coro::Generator<T> iota(size_t count, T start = T{0}, T step = T{1}) {
    for (auto i = 0; i < count; ++i, start += step)
	co_yield start;
    co_return;
}

}; // costr
