// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Apply `function` to each element of the given `source` returning
/// the number of elements.
///
/// \tparam S An input source that satisfies the **Stream** concept.
/// \tparam F A function that accepts a Stream element as input.
template<Stream S, class F>
size_t apply(S source, F function) {
    size_t count{0};
    for (auto&& value : source) {
	function(value);
	++count;
    }
    return count;
}

/// Apply `function` to each element of a Stream and return the number
/// of elements.
///
/// Returns a function that accepts an input Stream and applies the
/// given `function` to each element returning the number of elements.
///
/// \tparam F A function that accepts a Stream element as input
template<class F>
auto apply(F function) {
    return [=]<Stream S>(S&& source) {
	return apply<S>(std::forward<S>(source), std::move(function));
    };
}

}; // coro
