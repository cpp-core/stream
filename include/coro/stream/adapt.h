// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Adapt the supplied container to be a stream source.
template<class C>
Generator<stream_yield_t<C>> adapt(C& container) {
    for (auto&& elem : container)
	co_yield elem;
    co_return;
}

}; // costr
