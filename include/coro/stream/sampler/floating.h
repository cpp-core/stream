// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "coro/stream/detail/random.h"
#include "core/mp/traits/extrema.h"

namespace coro {

template<class T>
requires std::is_floating_point_v<T>
struct Sampler<T> {
    coro::Generator<T> operator()(T min = core::mp::extrema<T>::min(),
				  T max = core::mp::extrema<T>::max()) const {
	std::uniform_real_distribution<T> dist(0.0, 1.0);
	while (true) {
	    auto x = dist(coro::detail::rng());
	    T y = x * max + (1.0 - x) * min;
	    co_yield y;
	}
	co_return;
    }
};

}; // costr
