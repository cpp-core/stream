// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/tuple/from_vector.h"
#include "core/tuple/generate.h"

namespace coro {

// Yield values from the supplied `generator` grouped into
// **std::tuple**'s of `N` elements of type `T`.
template<size_t N, class T>
Generator<core::tp::generate_t<T, N>> group_tuple(Generator<T> generator) {
    std::vector<T> data;
    while (generator.next()) {
	data.push_back(generator());
	if (data.size() == N) {
	    co_yield core::tp::from_vector<T,N>(data);
	    data.clear();
	}
    }
    co_return;
}

// Return a unary group operator with a fixed `count`.
template<size_t N>
auto group_tuple() {
    return [=]<class G>(G&& g) {
	return group_tuple<N>(std::move(g));
    };
}

}; // costr
