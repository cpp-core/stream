// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/mp/same.h"

namespace costr {

template<class T, template<class...> class C>
constexpr bool is_kind = core::mp::is_same_template_v<T,C>;

template<class T>
using Gen = coro::Generator<T>;

template<class T>
coro::Generator<T> nil() {
    co_return;
}

template<class T>
coro::Generator<T> range(T start, T stop, T step = T{1}) {
    for (auto x = start; x <= stop; x += step)
	co_yield x;
    co_return;
}

template<class T>
coro::Generator<T> iota(size_t count, T step = T{1}, T start = T{0}) {
    for (auto i = 0; i < count; ++i, start += step)
	co_yield start;
    co_return;
}

template<class T>
vector<T> to_container(coro::Generator<T> generator) {
    vector<T> c;
    for (auto&& value : generator)
	c.emplace_back(value);
    return c;
}

}; // costr
