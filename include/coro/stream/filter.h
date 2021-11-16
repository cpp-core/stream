// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Return a **Stream** that filters the elements from `source` with `predicate`.
template<Stream S, class P>
Generator<typename S::value_type> filter(S source, P&& predicate) {
    for (auto element : source)
	if (predicate(element))
	    co_yield element;
    co_return;
}

// Filter a **Stream** using the given `predicate`.
//
// Return a function that accepts a **Stream** and returns a new **Stream** that filters
// the elements of the original **Stream** with the given `predicate`.
//
// *iota<int>(0, 10) | filter([](int n) { return n % 2 == 0; }); // 0, 2, 4, 6, 8*
template<class P>
auto filter(P predicate) {
    return [=]<Stream S>(S&& s) {
	return filter(std::forward<S>(s), predicate);
    };
}

}; // coro
