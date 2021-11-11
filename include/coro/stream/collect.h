// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace coro {

// Return a container **C<T>** with all the elements yielded from the
// supplied `generator`.
template<class C, class T>
auto collect(Generator<T> generator) {
    C c;
    for (auto&& value : generator)
	c.push_back(value);
    return c;
}

// Return a function that accepts **Generator<`T`>** and returns a
// container **C<`T`>** with all the elements yielded from the
// generator.
template<template<class...> class C>
auto collect() {
    return []<class T>(Generator<T>&& g) {
	return collect<C<T>>(std::forward<Generator<T>>(g));
    };
}

}; // coro
