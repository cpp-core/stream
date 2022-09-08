// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/util/random.h"

namespace coro {

/// Return a generator that yields elements from the supplied
/// `container`.
template<class C>
Generator<stream_yield_t<C>> draw(C& container) {
    auto dist = std::uniform_int_distribution<size_t>(0, container.size() - 1);
    while (true) {
	auto idx = dist.operator()(core::rng());
	co_yield container[idx];
    }
    co_return;
}

}; // costr
