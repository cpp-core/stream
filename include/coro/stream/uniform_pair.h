// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/uniform.h"
#include "core/algo/random.h"
#include "core/traits/extrema.h"
#include "core/mp/same.h"

namespace costr {

template<class T>
requires core::mp::is_same_template_v<T, std::pair>
struct Uniform<T> {
    using G = coro::Generator<T>;
    using First = typename T::first_type;
    using FirstG = coro::Generator<First>;
    using Second = typename T::second_type;
    using SecondG = coro::Generator<Second>;

    static T min_pair() {
	return std::make_pair(core::extrema<First>::min(), core::extrema<Second>::min());
    }
    
    static T max_pair() {
	return std::make_pair(core::extrema<First>::max(), core::extrema<Second>::max());
    }

    G operator()(FirstG g_first, SecondG g_second) const {
	auto iter_first = g_first.begin();
	auto iter_second = g_second.begin();
	while (iter_first != g_first.end() and iter_second != g_second.end()) {
	    T pair{*iter_first, *iter_second};
	    co_yield pair;
	    ++iter_first;
	    ++iter_second;
	}
	co_return;
    }
    
    G operator()(T min = min_pair(), T max = max_pair()) const {
	auto g_first = uniform<First>(min.first, max.first);
	auto g_second = uniform<Second>(min.second, max.second);
	return this->operator()(std::move(g_first), std::move(g_second));
    }
};

}; // costr
