// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "core/chrono.h"
#include "core/mp/same.h"
#include "core/algo/random.h"

namespace costr {

template<>
struct Sampler<chron::Date> {
    using Date = chron::Date;
    coro::Generator<Date> operator()(Date start = chron::jan/1/1990,
				     Date end = chron::jan/1/2040) const;
};

template<class T>
requires core::mp::is_same_template_v<T, std::chrono::time_point>
struct Sampler<T> {
    // coro::Generator<T> operator()(T start = T::min(), T end = T::max()) const;

    coro::Generator<T> operator()(T start = T::min(), T end = T::max()) const {
	auto sn = start.time_since_epoch().count();
	auto en = end.time_since_epoch().count();
	auto dist = std::uniform_int_distribution<int>(0, en - sn);
	while (true) {
	    auto n = sn + dist(core::rng());
	    auto tp = chron::timepoint_from_count<T, typename T::duration>(n);
	    co_yield tp;
	}
	co_return;
    }
};

template<class T>
requires core::mp::is_same_template_v<T, std::chrono::duration>
struct Sampler<T> {
    coro::Generator<T> operator()(T start = T::min(), T end = T::max()) const {
	auto sn = start.count();
	auto en = end.count();
	auto dist = std::uniform_int_distribution<int>(0, en - sn);
	while (true) {
	    auto n = sn + dist(core::rng());
	    T duration{n};
	    co_yield duration;
	}
	co_return;
    }
};

}; // costr
