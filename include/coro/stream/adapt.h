// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Return a generator that yields elements from the supplied
// `container`.
template<class C>
Generator<std::add_rvalue_reference_t<typename std::decay_t<C>::value_type>>
adapt(C& container) {
    for (auto&& elem : container)
	co_yield elem;
    co_return;
}

}; // costr
