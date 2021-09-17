// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace cogen {

template<class T>
coro::Generator<T> constant(T value) {
    while (true)
	co_yield value;
    co_return;
}

}; // cogen