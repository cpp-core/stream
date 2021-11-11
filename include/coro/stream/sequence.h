// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

template<class T, size_t N>
Generator<T> sequence(std::array<Generator<T>,N> arr) {
    for (auto i = 0; i < arr.size(); ++i)
	for (auto elem : arr[i])
	    co_yield elem;
    co_return;
}

inline auto sequence() {
    return []<class G>(G g) {
	return sequence(std::move(g));
    };
}

}; // coro
