// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "coro/stream/detail/random.h"
#include "core/mp/traits/extrema.h"

namespace coro {

template<class T>
requires std::is_integral_v<T>
struct Sampler<T> {
    coro::Generator<T> operator()(T min = core::mp::extrema<T>::min(),
				  T max = core::mp::extrema<T>::max()) const {
	std::uniform_int_distribution<T> dist(min, max);
	while (true)
	    co_yield dist(coro::detail::rng());
	co_return;
    }
};

}; // costr
