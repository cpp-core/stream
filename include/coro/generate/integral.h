// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/integral_decl.h"
#include "core/algo/random.h"

namespace cogen {

template<class T>
requires std::is_integral_v<T>
coro::Generator<T> uniform(T min, T max) {
    std::uniform_int_distribution<T> distribution(min, max);
    while (true)
	co_yield distribution(core::rng());
    co_return;
}

}; // cogen
