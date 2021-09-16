// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/decl.h"

namespace cogen {

template<class T, class U>
requires std::is_same_v<vector<U>, T>
coro::Generator<T> uniform(coro::Generator<U> g_elem, coro::Generator<size_t> g_size) {
    auto iter_elem = g_elem.begin();
    auto iter_size = g_size.begin();

    if (iter_elem == g_elem.end()) {
	auto g = uniform<U>();
	g_elem = g;
	iter_elem = g_elem.begin();
    }
	
    while (true) {
	vector<U> vec;
	auto count = *++iter_size;
	for (auto i = 0; i < count; ++i, ++iter_elem)
	    vec.push_back(*iter_elem);
	co_yield vec;
    }
    co_return;
}

template<class T, class U>
requires std::is_same_v<vector<U>, T>
coro::Generator<T> uniform(const U& min, const U& max, size_t min_count, size_t max_count) {
    return uniform<T>(uniform(min, max), uniform(min_count, max_count));
}

}; // cogen
