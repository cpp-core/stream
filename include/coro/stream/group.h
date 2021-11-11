// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "coro/stream/constant.h"
#include "core/tuple/from_vector.h"
#include "core/tuple/generate.h"

namespace coro {

// Return a generator that yields **std::vector<T>**'s. For each count
// yielded fromm the supplied **Generator<size_t>**`gsize`, the
// generator yields a vector of `T` with count elements yielded from
// the supplied **Generator<`T`>** `generator`.
template<class T>
Generator<std::vector<T>> group(Generator<T> generator, Generator<size_t> gsize) {
    std::vector<T> data;
    while (gsize.next()) {
	auto count = gsize();
	while (data.size() < count and generator.next())
	    data.push_back(generator());
	if (data.size() != count)
	    break;
	co_yield data;
	data.clear();
    }
    co_return;
}

// Return a function that accepts generators **G** and **H** and
// returns a new `group` generator. The new `group` generator yields
// **std::vector<`G::value_type`>**'s. For each count yielded from
// **H**, the generator yields a vector with count elements yielded
// from **G**.
auto group() {
    return [=]<class G, class H>(std::tuple<G,H>&& tup) {
	return group(std::move(std::get<0>(tup)), std::move(std::get<1>(tup)));
    };
}

// Return a function that accepts a generator **G** and returns a new
// `group` generator. The new `group` generator yields
// **std::vector<`G::value_type`>**'s with the size `count`
// constructed from the elements yielded from **G**.
auto group(size_t count) {
    return [=]<class G>(G&& g) {
	return group(std::move(g), constant(count));
    };
}

}; // coro
