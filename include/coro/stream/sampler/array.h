// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"

namespace coro {

template<class T, size_t N>
struct Sampler<std::array<T, N>> {
    using Type = std::array<T, N>;
    using G = Generator<Type>;
    using ElemG = Generator<T>;

    G operator()(ElemG g_elems) const {
	auto iter = g_elems.begin();
	while (iter != g_elems.end()) {
	    Type arr;
	    for (auto i = 0; i < arr.size(); ++i, ++iter)
		arr[i] = *iter;
	    co_yield arr;
	}
	co_return;
    }
    
    G operator()(T min, T max) const {
	auto g_elems = Sampler<T>{}(min, max);
	return this->operator()(std::move(g_elems));
    }
    
    G operator()() const {
	auto g_elems = Sampler<T>{}();
	return this->operator()(std::move(g_elems));
    }
};

}; // coro
