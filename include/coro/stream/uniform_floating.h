// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/uniform.h"
#include "core/algo/random.h"
#include "core/traits/extrema.h"

namespace costr {

template<class T>
requires std::is_floating_point_v<T>
struct Sampler<T> {
    coro::Generator<T> operator()(T min = core::extrema<T>::min(),
				  T max = core::extrema<T>::max()) const {
	std::uniform_real_distribution<T> dist(0.0, 1.0);
	while (true) {
	    auto x = dist(core::rng());
	    T y = x * max + (1.0 - x) * min;
	    co_yield y;
	}
	co_return;
    }
};

}; // costr
