// Copyright 2021, 2022, 2023 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"
#include "core/tuple/fold.h"
#include "core/tuple/map.h"

namespace coro {

template<class... Ts>
struct Sampler<std::tuple<Ts...>> {
    using T = std::tuple<Ts...>;
    using G = coro::Generator<T>;

    G operator()() const {
	using core::tp::map, core::tp::map_inplace, core::tp::all;
	auto g_tup = std::make_tuple(Sampler<Ts>{}()...);
	auto iterators = map_inplace([](auto& g) { return g.begin(); }, g_tup);
	auto end_iters = map_inplace([](auto& g) { return g.end(); }, g_tup);
	while (all(map([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	    co_yield map([](auto& iter) { return *iter; }, iterators);
	    map_inplace([](auto& iter) { ++iter; }, iterators);
	}
	co_return;
    }
    
    // G operator()(T min, T max) const {
    // 	auto g_first = Sampler<First>{}(min.first, max.first);
    // 	auto g_second = Sampler<Second>{}(min.second, max.second);
    // 	return this->operator()(std::move(g_first), std::move(g_second));
    // }
    
    // G operator()() const {
    // 	std::cout << this << std::endl;
    // 	return this->operator()(Sampler<Ts>{}()...);
    // }
};

// template<class T, class U>
// auto sampler_tuple(std::pair<T,U>&& min, std::pair<T,U>&& max) {
//     using Pair = std::pair<T,U>;
//     return Sampler<Pair>{}(std::forward<Pair>(min), std::forward<Pair>(max));
// }

}; // coro
