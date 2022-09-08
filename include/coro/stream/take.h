// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Return a generator that yields the first `count` elements (or until
/// exhaustion) from the supplied `generator`.
template<Stream S>
Generator<stream_yield_t<S>> take(S source, size_t count) {
    if (count > 0) {
	for (auto&& elem : source) {
	    co_yield elem;
	    if (--count == 0)
		break;
	}
    }
    co_return;
}

/// Take the first `count` elements from the preceeding generator.
///
/// Usage: *sampler<int>() | take(10)*
inline auto take(size_t count) {
    return [=]<Stream S>(S&& source) {
	return take<S>(std::forward<S>(source), count);
    };
}

}; // coro
