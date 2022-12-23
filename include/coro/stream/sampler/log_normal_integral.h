// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/log_normal_sampler.h"
#include "coro/stream/sampler/integral.h"
#include "coro/stream/detail/random.h"

namespace coro {

template<class T>
requires (std::is_integral_v<T> and sizeof(T) < 16)
struct LogNormalSampler<T> {
    coro::Generator<T> operator()(size_t min_bits = 0,
				  size_t max_bits = sizeof(T) * CHAR_BIT) const {
	using U = std::make_unsigned_t<T>;
	auto available_bits = sizeof(U) * CHAR_BIT;
	max_bits = std::min(max_bits, available_bits);
	
	std::uniform_int_distribution<U> dist;
	for (auto n : sampler<size_t>(min_bits, max_bits)) {
	    auto mask = n > 0 ? (U{1} << (n - 1)) : 0;
	    mask |= mask - 1;
	    auto mag = dist(coro::detail::rng()) bitand mask;
	    if (std::is_signed_v<T> and dist(coro::detail::rng()) bitand 0x100) {
		mag = ~mag;
		mag += 1;
	    }
	    T value(mag);
	    co_yield value;
	}
	co_return;
    }
};

template<class T>
requires (std::is_integral_v<T> and sizeof(T) == 16)
struct LogNormalSampler<T> {
    coro::Generator<T> operator()(size_t min_bits = 0,
				  size_t max_bits = sizeof(T) * CHAR_BIT) const {
	using U = std::make_unsigned_t<T>;
	auto available_bits = sizeof(U) * CHAR_BIT;
	max_bits = std::min(max_bits, available_bits);
	
	std::uniform_int_distribution<T> dist;
	for (auto n : sampler<size_t>(min_bits, max_bits)) {
	    auto mask = n > 0 ? (U{1} << (n - 1)) : 0;
	    mask |= mask - 1;
	    
	    U mag = dist(coro::detail::rng());
	    mag <<= 64;
	    mag |= dist(coro::detail::rng());
	    mag &= mask;
	    
	    if (std::is_signed_v<T> and dist(coro::detail::rng()) bitand 0x100) {
		mag = ~mag;
		mag += 1;
	    }
	    
	    T value(mag);
	    co_yield value;
	}

	co_return;
    }
};

}; // costr
