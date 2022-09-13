// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/detail/random.h"

namespace coro {

/// Return a generator that yields elements from the supplied
/// `container`.
template<class C>
Generator<stream_yield_t<C>> draw(C& container) {
    auto dist = std::uniform_int_distribution<std::size_t>(0, container.size() - 1);
    while (true) {
	auto idx = dist.operator()(coro::detail::rng());
	co_yield container[idx];
    }
    co_return;
}

}; // costr
