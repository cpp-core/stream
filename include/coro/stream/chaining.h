// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Return an **array<`G`,2>** containing the generators `left` and `right`. This allows
// for the composition of a pair of generators.
//
// *sampler<int>(0,9) + sampler<int>(10,19)*
template<Stream S>
auto operator+(S left, S right) {
    using T = typename std::decay_t<S>::value_type;
    return std::array<Generator<T>,2>{std::move(left), std::move(right)};
}

// Return an **array<`G`,`N`+1>** containing the generators from `arr` and the generator
// `right`. Coupled with pairwise composition, this allows for the composition of any
// number of generators:
//
// *sampler<int>(0,9) & sampler<int>(10,19) & sampler<int>(20,29)*
template<Stream S, class T, size_t N>
auto operator+(std::array<T,N> arr, S right) {
    std::array<T,N+1> arr1;
    for (auto i = 0; i < N; ++i)
	arr1[i] = arr[i];
    arr1[N] = right;
    return arr1;
}

// Return a **std::tuple** of the **Stream**'s `l` and `r`.
//
// *sampler<int>(0, 10) x sampler<int>(0, 5) | zip() | take(5)* 
template<Stream L, Stream R>
auto operator*(L&& l, R&& r) {
    return std::tuple<L,R>{std::forward<L>(l), std::forward<R>(r)};
}

// Return a the given **Stream** tuple `ls` with the **Stream** `r` appended.  `g`.
//
// *sampler<int>(0, 10) x sampler<int>(0, 5) x sampler<int>(-10, 0) | zip() | take(5)* 
template<Stream... Ls, Stream R>
auto operator*(std::tuple<Ls...>&& ls, R&& r) {
    return std::tuple_cat(std::move(ls), std::tuple<R>{std::forward<R>(r)});
}

// Return the result of applying operation `op` to the **Stream** `s`.
//
// *sampler<int>(0, 100) | take(4) | collect<std::vector>()*
template<Stream S, class Op>
auto operator|(S&& s, Op&& op) {
    return op(std::forward<S>(s));
}

// Return the result of applying operation `op` to the array of streams `arr`.
//
// *sampler<int>(0,9) & sampler<int>(10,19) | sequence()*
template<Stream S, size_t N, class Op>
auto operator|(std::array<S,N> arr, Op op) {
    return op(std::move(arr));
}

// Return the result of applying operation `op` to the tuple of **Stream**'s `tup`.
//
// *sampler<int>(0, 10) * sampler<int>(0, 5) | zip() | take(5)* 
template<class Op, Stream... Ss>
auto operator|(std::tuple<Ss...>&& tup, Op&& op) {
    return op(std::forward<std::tuple<Ss...>>(tup));
}

}; // coro
