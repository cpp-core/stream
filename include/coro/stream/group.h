// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/repeat.h"
#include "core/tuple/from_vector.h"

namespace coro {

/// Return a generator that yields **std::vector<T>**'s. For each count yielded fromm the
/// supplied **Generator<size_t>**`gsize`, the generator yields a vector of `T` with count
/// elements yielded from the supplied **Generator<`T`>** `generator`.
template<Stream S, Stream R>
Generator<std::vector<stream_value_t<S>>&&> group(S source, R sizer) {
    std::vector<stream_value_t<S>> data;
    auto iter = std::begin(source);
    auto end = std::end(source);
    
    for (auto count : sizer) {
	while (count > 0 and iter != end) {
	    data.push_back(*iter);
	    ++iter;
	    --count;
	}
	if (data.size() < count)
	    break;
	co_yield data;
	data.clear();
    }
    co_return;
}

/// Group elements into vectors based on a 2-tuple of generators. Group the elements from
/// the first generator into vectors of length determined by the elements of the second
/// generator.
///
/// Return a function that accepts generators **G** and **H** and returns a new `group`
/// generator. The new `group` generator yields **std::vector<`G::value_type`>**'s. For
/// each count yielded from **H**, the generator yields a vector with count elements
/// yielded from **G**.
inline auto group() {
    return [=]<class G, class H>(std::tuple<G,H>&& tup) {
	return group(std::move(std::get<0>(tup)), std::move(std::get<1>(tup)));
    };
}

/// Group elements from a generator into vectors of length `count`.
///
/// Returns: a function that accepts a generator **G** and returns a new
/// `group` generator. The new `group` generator yields
/// **std::vector<`G::value_type`>**'s with the size `count`
/// constructed from the elements yielded from **G**.
inline auto group(size_t count) {
    return [=]<class G>(G&& g) {
	return group(std::move(g), repeat(count));
    };
}

}; // coro
