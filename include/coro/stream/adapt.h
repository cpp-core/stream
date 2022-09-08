// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Adapts the supplied `container` to be a Stream source.
///
/// Returns a Generator that yields the elements of the given
/// `container`.
///
/// \tparam C A container that is range-based for-loop enabled.
template<class C>
Generator<stream_yield_t<C>> adapt(C& container) {
    for (auto&& elem : container)
	co_yield elem;
    co_return;
}

}; // coro
