// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Returns a **std::tuple** of the given Streams `l` and `r`.
///
/// \rst
/// ```{code-block} c++
/// sampler<int>(0, 10) * sampler<int>(0, 5) | zip() | take(5)
/// ```
/// \endrst
///
/// \tparam L A source that satisfies the `Stream` concept.
/// \tparam R A source that satisfies the `Stream` concept.
template<Stream L, Stream R>
auto operator*(L&& l, R&& r) {
    return std::tuple<L,R>{std::forward<L>(l), std::forward<R>(r)};
}

/// Returns a new tuple consisting of `tuple` with `stream` appending.
///
/// \rst
/// ```{code-block} c++
/// sampler<int>(0, 10) * sampler<int>(0, 5) * sampler<int>(5, 10) | zip() | take(5)
/// ```
/// \endrst
///
/// \tparam Ss One or more source(s) that satisfies the `Stream` concept.
/// \tparam S A source that satisfies the `Stream` concept.
template<Stream... Ss, Stream S>
auto operator*(std::tuple<Ss...>&& tuple, S&& stream) {
    return std::tuple_cat(std::move(tuple), std::tuple<S>{std::forward<S>(stream)});
}

/// Returns the result of applying operation `op` to the Stream `s`
///
/// In the following code snippet, `S` is the sampler and `Op` is the
/// take operation. The pipe operator applies the function that is
/// returned from `take(4)` to the sampler to get the resulting
/// Stream.
///
/// \rst
/// ```{code-block} c++
/// sampler<int>(0, 100) | take(4);
/// // maybe 17, 65, 33, 87;
/// ```
/// \endrst
///
/// \tparam S A source that satisfies the **Stream** concept.
/// \tparam Op A functor that takes a Stream argument.
template<Stream S, class Op>
auto operator|(S&& s, Op&& op) {
    return op(std::forward<S>(s));
}

/// Returns the result of applying operation `op` to the tuple of Streams `tuple`.
///
/// \rst
/// ```{code-block} c++
/// sampler<int>(0, 10) * sampler<int>(0, 5) | zip() | take(5)
/// // maybe (7,1), (6,0), (3,3), (1, 3);
/// ```
/// \endrst
///
/// \tparam S A tuple of sources that all satisfy the **Stream** concept.
/// \tparam Op A functor that takes a tuple of Stream argument.
template<class Op, Stream... Ss>
auto operator|(std::tuple<Ss...>&& tuple, Op&& op) {
    return op(std::forward<std::tuple<Ss...>>(tuple));
}

}; // coro
