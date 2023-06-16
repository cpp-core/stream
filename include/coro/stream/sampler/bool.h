// Copyright 2021, 2022, 2023 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "coro/stream/detail/random.h"

namespace coro {

template<>
struct Sampler<bool> {
    Generator<bool> operator()() const {
	std::uniform_int_distribution<int> dist(0, 1);
	while (true)
	    co_yield dist(coro::detail::rng());
	co_return;
    }
};

}; // coro
