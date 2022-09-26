// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "coro/stream/detail/random.h"

namespace coro {

template<class T>
requires std::is_integral_v<T>
struct Sampler<T> {
    coro::Generator<T> operator()(T min = std::numeric_limits<T>::min(),
				  T max = std::numeric_limits<T>::max()) const {
	std::uniform_int_distribution<T> dist(min, max);
	while (true)
	    co_yield dist(coro::detail::rng());
	co_return;
    }
};

}; // costr
