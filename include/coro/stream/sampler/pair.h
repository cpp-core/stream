// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "core/util/random.h"
#include "core/mp/traits/extrema.h"
#include "core/mp/same.h"

namespace coro {

template<class T>
requires core::mp::is_same_template_v<T, std::pair>
struct Sampler<T> {
    using G = coro::Generator<T>;
    
    using First = typename T::first_type;
    using FirstRef = const First&;
    using FirstG = coro::Generator<First>;
    
    using Second = typename T::second_type;
    using SecondRef = const Second&;
    using SecondG = coro::Generator<Second>;

    static std::pair<First,First> min_pair() {
	return std::make_pair(core::mp::extrema<First>::min(), core::mp::extrema<Second>::min());
    }
    
    static std::pair<Second,Second> max_pair() {
	return std::make_pair(core::mp::extrema<First>::max(), core::mp::extrema<Second>::max());
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
    
    G operator()(T min, T max) const {
	auto g_first = Sampler<First>{}(min.first, max.first);
	auto g_second = Sampler<Second>{}(min.second, max.second);
	return this->operator()(std::move(g_first), std::move(g_second));
    }
    
    G operator()() const {
	auto g_first = Sampler<First>{}();
	auto g_second = Sampler<Second>{}();
	return this->operator()(std::move(g_first), std::move(g_second));
    }
};

template<class T, class U>
auto sampler_pair(std::pair<T,U>&& min, std::pair<T,U>&& max) {
    using Pair = std::pair<T,U>;
    return Sampler<Pair>{}(std::forward<Pair>(min), std::forward<Pair>(max));
}

}; // coro
