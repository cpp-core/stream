// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace costr {

template<class T>
coro::Generator<T> take(coro::Generator<T> generator, size_t count) {
    while (count-- > 0 and generator.next())
	co_yield generator();
    co_return;
}

}; // costr
