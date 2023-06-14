// Copyright 2021, 2022, 2023 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "coro/stream/detail/random.h"

namespace coro {

template<class T> requires std::is_integral_v<T>
struct Sampler<T> {
    template<class U> requires std::is_integral_v<U> struct uniform_dist_type { using type = U; };
    template<> struct uniform_dist_type<char> { using type = int8_t; };
    template<> struct uniform_dist_type<signed char> { using type = int8_t; };
    template<> struct uniform_dist_type<unsigned char> { using type = uint8_t; };
    
    Generator<T> operator()(T min = std::numeric_limits<T>::min(),
			    T max = std::numeric_limits<T>::max()) const {
	using U = typename uniform_dist_type<std::remove_cvref_t<T>>::type;
	std::uniform_int_distribution<U> dist(min, max);
	while (true)
	    co_yield dist(coro::detail::rng());
	co_return;
    }

    static size_t clamp(size_t n, size_t min, size_t max) {
	return std::min(std::max(n, min), max);
    }

    using U = std::make_unsigned_t<std::decay_t<T>>;
    static constexpr auto available_bits = sizeof(U) * CHAR_BIT;
    
    Generator<U> log_normal_magnitude(size_t min_bits, size_t max_bits) const {
	using W = typename uniform_dist_type<std::remove_cvref_t<U>>::type;
	if (min_bits > max_bits)
	    std::swap(min_bits, max_bits);
	max_bits = clamp(max_bits, 1ul, available_bits);
	min_bits = clamp(min_bits, 1ul, available_bits);
	
	std::uniform_int_distribution<W> dist;
	for (auto n : sampler<size_t>(min_bits, max_bits)) {
	    auto mask = U{1} << (n - 1);
	    mask |= mask - 1;
	    co_yield dist(coro::detail::rng()) bitand mask;
	}
	co_return;
    }

    Generator<U> log_normal_magnitude(size_t max_bits) const {
	return this->log_normal_magnitude(1ul, max_bits);
    }

    Generator<U> log_normal_magnitude() const {
	return this->log_normal_magnitude(1ul, available_bits);
    }

    Generator<T> log_normal(size_t min_bits, size_t max_bits) const {
	std::uniform_int_distribution<int> dist;
	for (auto mag : this->log_normal_magnitude(min_bits, max_bits)) {
	    if (std::is_signed_v<T> and dist(coro::detail::rng()) bitand 0x100) {
		mag = ~mag;
		mag += 1;
	    }
	    co_yield T(mag);
	}
	co_return;
    }

    Generator<T> log_normal() const {
	return this->log_normal(1ul, available_bits);
    }
    
    Generator<T> log_normal(size_t max_bits) const {
	return this->log_normal(1ul, max_bits);
    }
};

}; // coro
