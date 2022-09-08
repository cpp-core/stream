// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

/// Return a **std::tuple** of the **Stream**'s `l` and `r`.
///
/// *sampler<int>(0, 10) x sampler<int>(0, 5) | zip() | take(5)* 
template<Stream L, Stream R>
auto operator*(L&& l, R&& r) {
    return std::tuple<L,R>{std::forward<L>(l), std::forward<R>(r)};
}

/// Return the **Stream** `r` appended to the tuple of **Stream**'s `ls`.
///
/// *sampler<int>(0, 10) x sampler<int>(0, 5) x sampler<int>(-10, 0) | zip() | take(5)* 
template<Stream... Ss, Stream S>
auto operator*(std::tuple<Ss...>&& tuple, S&& stream) {
    return std::tuple_cat(std::move(tuple), std::tuple<S>{std::forward<S>(stream)});
}

/// Return the result of applying operation `op` to the **Stream** `s`.
///
/// *sampler<int>(0, 100) | take(4) | collect<std::vector>()*
template<Stream S, class Op>
auto operator|(S&& s, Op&& op) {
    return op(std::forward<S>(s));
}

/// Return the result of applying operation `op` to the tuple of **Stream**'s `tup`.
///
/// *sampler<int>(0, 10) * sampler<int>(0, 5) | zip() | take(5)* 
template<class Op, Stream... Ss>
auto operator|(std::tuple<Ss...>&& tup, Op&& op) {
    return op(std::forward<std::tuple<Ss...>>(tup));
}

}; // coro
