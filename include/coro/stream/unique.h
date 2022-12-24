// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <set>
#include "coro/stream/util.h"

namespace coro {

/// Return a generator that yields the elements from the supplied `generator` filtering
/// them to be unique with respect to the given `key` function.
template<Stream S, class F>
Generator<stream_yield_t<S>> unique(S source, F key) {
    using T = stream_value_t<S>;
    using K = std::invoke_result_t<F, T>;
    std::set<K> s;
    for (auto&& element : source) {
	auto k = key(element);
	if (not s.contains(k)) {
	    co_yield element;
	    s.insert(k);
	}
    }
    co_return;
}

/// Filter elements from a generator to make them unique with respect to the given `key`
/// function which defaults to identity.
///
/// Return a function that accepts a generator **G** and returns a new
/// `unique` generator. The `unique` generator yields the elements of
/// **G** filtered with respect to the given `key` function.
///
/// *sampler<int>(0, 100) | unique([](int n) { return n % 11; })*
template<class F>
auto unique(F key) {
    return [=]<Stream S>(S&& source) {
	return unique<S>(std::forward<S>(source), std::move(key));
    };
}

}; // coro
