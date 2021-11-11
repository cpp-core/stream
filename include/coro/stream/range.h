// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

template<class T>
coro::Generator<T> range(T start, T stop, T step = T{1}) {
    for (auto x = start; x <= stop; x += step)
	co_yield x;
    co_return;
}

}; // costr
