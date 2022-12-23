// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "coro/stream/detail/random.h"

namespace coro {

template<class T>
requires (std::is_integral_v<T> and sizeof(T) < 16)
struct Sampler<T> {
    coro::Generator<T> operator()(T min = std::numeric_limits<T>::min(),
				  T max = std::numeric_limits<T>::max()) const {
	std::uniform_int_distribution<T> dist(min, max);
	while (true)
	    co_yield dist(coro::detail::rng());
	co_return;
    }
};

template<class T>
requires (std::is_integral_v<T> and sizeof(T) == 16)
struct Sampler<T> {
    coro::Generator<T> operator()(T min = std::numeric_limits<T>::min(),
				  T max = std::numeric_limits<T>::max()) const {
	__uint128_t range = max - min;
	if (range <= std::numeric_limits<uint64_t>::max()) {
	    std::uniform_int_distribution<T> dist(0, range);
	    while (true) {
		T value = min + (T)dist(coro::detail::rng());
		co_yield value;
	    }
	} else {
	    std::uniform_int_distribution<T> dist;
	    while (true) {
		T value = dist(coro::detail::rng());
		value <<= 64;
		value |= dist(coro::detail::rng());
		value %= range;
		value += min;
		co_yield value;
	    }
	}

	co_return;
    }
};

}; // costr
