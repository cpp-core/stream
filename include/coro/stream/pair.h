// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/decl.h"

namespace costr {

template<class T, class U, class V>
requires is_kind<T, std::pair>
coro::Generator<T> sample(coro::Generator<U> first, coro::Generator<V> second) {
    auto iter_first = first.begin();
    auto iter_second = second.begin();
    while (iter_first != first.end() and iter_second != second.end()) {
	co_yield std::make_pair(*iter_first, *iter_second);
	++iter_first;
	++iter_second;
    }
}

template<class T, class U, class V>
requires is_kind<T, std::pair>
coro::Generator<T> uniform() {
    return sample<T>(uniform<U>(), uniform<V>());
}

template<class T, class U, class V>
requires is_kind<T, std::pair>
coro::Generator<T> uniform(const U& first_min, const U& first_max,
			   const V& second_min, const V& second_max) {
    return sample<T>(uniform<U>(first_min, first_max), uniform<V>(second_min, second_max));
}

}; // costr
