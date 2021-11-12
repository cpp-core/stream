// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

// Return an **array<`G`,2>** containing the generators `left` and `right`. This allows
// for the composition of a pair of generators.
//
// *sampler<int>(0,9) + sampler<int>(10,19)*
template<class G>
requires is_generator_v<G>
auto operator+(G left, G right) {
    using T = typename std::decay_t<G>::value_type;
    return std::array<Generator<T>,2>{std::move(left), std::move(right)};
}

// Return an **array<`G`,`N`+1>** containing the generators from `arr` and the generator
// `right`. Coupled with pairwise composition, this allows for the composition of any
// number of generators:
//
// *sampler<int>(0,9) & sampler<int>(10,19) & sampler<int>(20,29)*
template<class G, class T, size_t N>
requires is_generator_v<G>
auto operator+(std::array<T,N> arr, G right) {
    std::array<T,N+1> arr1;
    for (auto i = 0; i < N; ++i)
	arr1[i] = arr[i];
    arr1[N] = right;
    return arr1;
}

// Return a **tuple<`G`,`H`>** containing the generators `left` and `right`.
//
// *sampler<int>(0, 10) x sampler<int>(0, 5) | zip() | take(5)* 
template<class G, class H>
requires (is_generator_v<G> && is_generator_v<H>)
auto operator*(G&& g, H&& h) {
    return std::tuple{std::forward<G>(g), std::forward<H>(h)};
}

// Return a **tuple<...>** containing the generators from `tup` and the generator from
// `g`.
//
// *sampler<int>(0, 10) x sampler<int>(0, 5) x sampler<int>(-10, 0) | zip() | take(5)* 
template<class G, class... Gs>
requires (is_generator_v<G> && (is_generator_v<Gs> && ...))
auto operator*(std::tuple<Gs...>&& tup, G&& g) {
    if constexpr (sizeof...(Gs) == 1)
        return std::tuple{std::move(std::get<0>(tup)),
	    std::forward<G>(g)};
    else if constexpr (sizeof...(Gs) == 2)
        return std::tuple{std::move(std::get<0>(tup)),
	    std::move(std::get<1>(tup)),
	    std::forward<G>(g)};
}

// Return the result of applying `op` to the generator `g`.
//
// *sampler<int>(0, 100) | take(4) | collect<std::vector>()*
template<class G, class Op>
requires is_generator_v<G>
auto operator|(G&& g, Op&& op) {
    return op(std::forward<G>(g));
}

// Return the result of applying `op` to the array of generators `arr`.
//
// *sampler<int>(0,9) & sampler<int>(10,19) | sequence()*
template<class G, size_t N, class Op>
requires is_generator_v<G>
auto operator|(std::array<G,N> arr, Op op) {
    return op(std::move(arr));
}

// Return the result of applying `op` to the tuple of generators `tup`.
//
// *sampler<int>(0, 10) ^ sampler<int>(0, 5) | zip() | take(5)* 
template<class Op, class... Gs>
requires (is_generator_v<Gs> && ...)
auto operator|(std::tuple<Gs...>&& tup, Op&& op) {
    return op(std::forward<std::tuple<Gs...>>(tup));
}

}; // coro
