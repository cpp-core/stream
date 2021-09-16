// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/floating_decl.h"
#include "core/algo/random.h"

namespace cogen {

template<class T>
requires std::is_floating_point_v<T>
coro::Generator<T> uniform(T min, T max) {
    std::uniform_real_distribution<T> distribution(0.0, 1.0);
    while (true) {
	auto x = distribution(core::rng());
	co_yield (T)(x * max + (1.0 - x) * min);
    }
    co_return;
}

}; // cogen
