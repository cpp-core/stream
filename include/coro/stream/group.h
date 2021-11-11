// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "coro/stream/constant.h"
#include "core/tuple/from_vector.h"
#include "core/tuple/generate.h"

namespace coro {

// Yield **std::vector<T>**'s with values yielded from the supplied
// `generator` and sizes determined by the values yielded from the
// generator `gsize`.
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

// Return a binary group operator.
auto group() {
    return [=]<class G, class H>(std::tuple<G,H>&& tup) {
	return group(std::move(std::get<0>(tup)), std::move(std::get<1>(tup)));
    };
}

auto group(size_t count) {
    return [=]<class G>(G&& g) {
	return group(std::move(g), constant(count));
    };
}

}; // costr
