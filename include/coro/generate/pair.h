// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/decl.h"

namespace cogen {

template<class T, class U, class V>
requires std::is_same_v<T, std::pair<U,V>>
coro::Generator<T> uniform(coro::Generator<U> g_first, coro::Generator<V> g_second) {
    auto iter_first = g_first.begin();
    auto iter_second = g_second.begin();

    if (iter_first == g_first.end()) {
	auto g = uniform<U>();
	g_first = g;
	iter_first = g_first.begin();
    }

    if (iter_second == g_second.end()) {
	auto g = uniform<V>();
	g_second = g;
	iter_second = g_second.begin();
    }

    while (true) 
	co_yield std::make_pair(*++iter_first, *++iter_second);

    co_return;
}

template<class T, class U, class V>
requires std::is_same_v<T, std::pair<U,V>>
coro::Generator<T> uniform(const U& first_min, const U& first_max,
			   const V& second_min, const V& second_max) {
    return uniform<T>(uniform(first_min, first_max), uniform(second_min, second_max));
}


}; // cogen
