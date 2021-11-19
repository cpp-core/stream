// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// 
// 
template<Stream S, class A, class R>
A reduce(S source, A accumulator, R&& reducer) {
    for (auto&& elem : source)
	reducer(accumulator, elem);
    return accumulator;
}

// 
// 
// 
// 
//
// 
template<class A, class R>
auto reduce(A&& accumulator, R&& reducer) {
    return [a = std::forward<A>(accumulator), r = std::forward<R>(reducer)]
	<Stream S>(S&& source) mutable {
	return reduce<S>(std::forward<S>(source),
			 std::forward<A>(a),
			 std::forward<R>(r));
    };
}

}; // coro
