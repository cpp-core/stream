// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Filter values from `source` using the given `predicate`.
///
/// Returns a **Generator** that yields only those values from `source`
/// for which `predicate` evaluates to true.
///
/// \tparam S An input source that satisfies the **Stream** concept.
/// \tparam P A predicate that evaluates to bool for a Stream element.
template<Stream S, class P>
Generator<stream_yield_t<S>> filter(S source, P&& predicate) {
    for (auto&& element : source)
	if (predicate(element))
	    co_yield element;
    co_return;
}

/// Filter values using the given `predicate`.
///
/// Returns a function that accepts an input Stream and returns a
/// new Stream that filters values using the given `predicate`.
///
/// \rst
/// ```{code-block} cpp
/// iota<int>(0, 10) | filter([](int n) { return n % 2 == 0; }); 
/// // 0, 2, 4, 6, 8
/// ```
/// \endrst
///
/// \tparam P A predcate that evalutes to bool for a Stream element.
template<class P>
auto filter(P predicate) {
    return [=]<Stream S>(S&& s) {
	return filter<S>(std::forward<S>(s), predicate);
    };
}

}; // coro
