// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace costr {

template<class C>
coro::Generator<typename C::value_type> to_stream(const C& container) {
    for (auto elem : container)
	co_yield elem;
    co_return;
}

}; // costr
