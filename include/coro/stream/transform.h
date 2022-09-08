// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Return a generator that yields the elements from the supplied `generator` transformed
/// by the given function `func` that maps **T** to **U**.
template<Stream S, class F, class U = std::result_of_t<F&(stream_value_t<S>)>>
Generator<U&&> transform(S source, F&& func) {
    for (auto&& elem : source)
	co_yield func(elem);
    co_return;
}

/// Transform the elements from the preceeding generator using the supplied `func`.
/// 
/// *Returns:* **Generator<...>** A generator that yields the values from the underlying
/// *generator transformed by `func`.
///
/// *sampler<int>(0, 10) | transform([](int n) { return n + 10; })*
template<class F>
auto transform(F&& function) {
    return [f = std::forward<F>(function)]<Stream S>(S&& source) mutable {
	return transform<S>(std::forward<S>(source), std::forward<F>(f));
    };
}

}; // coro
