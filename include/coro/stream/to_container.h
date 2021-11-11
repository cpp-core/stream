// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

template<class T>
vector<T> to_container(coro::Generator<T> generator) {
    vector<T> c;
    for (auto&& value : generator)
	c.emplace_back(value);
    return c;
}

auto to_container() {
    return []<class G>(G&& g) {
	return to_container(std::forward<G>(g));
    };
}

}; // costr
