// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Return a count of elements in **Stream** `source` after applying `function` to each
// element.
template<Stream S, class F>
size_t apply(S&& source, F&& function) {
    size_t count{0};
    for (auto&& value : source) {
	std::forward<F>(function)(value);
	++count;
    }
    return count;
}

// Apply `function` to each element of a **Stream** and return the count of elements.
//
// Return a function that accepts a **Stream** and returns the count of elements in the
// **Stream** after applying `function` to each element.
template<class F>
auto apply(F function) {
    return [=]<Stream S>(S&& source) {
	return apply(std::forward<S>(source), std::move(function));
    };
}

}; // coro
