// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "core/algo/random.h"
#include "core/traits/extrema.h"

namespace coro {

template<class T>
requires std::is_integral_v<T>
struct Sampler<T> {
    coro::Generator<T> operator()(T min = core::extrema<T>::min(),
				  T max = core::extrema<T>::max()) const {
	std::uniform_int_distribution<T> dist(min, max);
	while (true)
	    co_yield dist(core::rng());
	co_return;
    }
};

}; // costr
