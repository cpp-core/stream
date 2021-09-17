// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace costr {

template<class T, class F>
coro::Generator<T> filter(coro::Generator<T> generator, F&& func) {
    for (const auto& element : generator)
	if (func(element))
	    co_yield element;
    co_return;
}

}; // costr
