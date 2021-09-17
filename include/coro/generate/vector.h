// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/decl.h"

namespace cogen {

template<class T, class U>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(Gen<size_t> g_size, Gen<U> g_elem) {
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
Gen<T> uniform(size_t min_count, size_t max_count, const U& min, const U& max) {
    return uniform<T>(uniform(min_count, max_count), uniform(min, max));
}

template<class T, class U>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(size_t min_count, size_t max_count, Gen<U>&& g_elem) {
    return uniform<T>(uniform(min_count, max_count), std::forward<Gen<U>>(g_elem));
}

template<class T, class U>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(Gen<U>&& g_elem) {
    return uniform<T>(uniform<size_t>(0, 20), std::forward<Gen<U>>(g_elem));
}

}; // cogen
