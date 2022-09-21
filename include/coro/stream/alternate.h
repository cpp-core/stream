// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <optional>
#include "coro/stream/util.h"
#include "core/tuple/fold.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace coro {

/// Alternate elements from the given `tuple` of input Streams until
/// all are exhausted.
///
/// Returns a **Generator** that yields elements from the given
/// `tuple` of input Streams in an round-robin fashion unitl all are
/// exhausted.
///
/// \tparam S A source that satisfies the `Stream` concept.
/// \tparam Ss A source(s) that satisfies the `Stream` concept.
template<Stream S, Stream... Ss>
Generator<const stream_value_t<S>&> alternate(std::tuple<S, Ss...> tuple) {
    using namespace core::tp;
    using value_type = stream_value_t<S>;
    
    auto iterators = map_inplace([](auto& g) { return g.begin(); }, tuple);
    auto end_iters = map_inplace([](auto& g) { return g.end(); }, tuple);
    while (any(map([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	auto values = map([](auto& iter, auto& end) {
	    if (iter != end) return std::optional<value_type>{*iter};
	    else return std::optional<value_type>{};
	}, iterators, end_iters);
	auto arr = to_array(values);
	for (auto i = 0; i < std::tuple_size<decltype(values)>(); ++i)
	    if (arr[i])
		co_yield arr[i].value();
	map_inplace([](auto& iter, auto& end) { if (iter != end) ++iter; }, iterators, end_iters);
    }
    co_return;
}

/// Alternate elements from a tuple of Streams until all are
/// exhausted.
///
/// Returns a function that accepts a tuple of input Stream's and
/// returns a new Stream that yields elements from the input Streams
/// in a round-robin fashion until all are exhausted.
///
/// \rst
/// ```{code-block} c++
/// sampler<int>(0, 9) * sampler<int>(10, 19) * sampler<int>(20, 29) | alternate();
/// // 0, 10, 20, 1, 11, 21, 2, 12, 22, 3, 13, 23, ...
/// ```
/// \endrst
inline auto alternate() {
    return []<TupleOfStream S>(S&& source) {
	return alternate(std::move(source));
    };
}

}; // coro
