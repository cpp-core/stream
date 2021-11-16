// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Return a container **C<T>** with all the elements from **Stream** `source`.
template<class C, Stream S>
auto collect(S source) {
    C c;
    for (auto&& value : source)
	c.push_back(value);
    return c;
}

// Collect all the elements from a **Stream** and insert them into a container of type
// **C**.
//
// Return a function that accepts a **Stream** and returns a container **C<`T`>** with all
// the elements from the **Stream**.
template<template<class...> class C>
auto collect() {
    return []<Stream S>(S&& s) {
	using T = typename std::decay_t<S>::value_type;
	return collect<C<T>>(std::forward<S>(s));
    };
}

}; // coro
