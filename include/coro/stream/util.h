// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/mp/same.h"

namespace coro {

template<class T, template<class...> class C>
constexpr bool is_kind = core::mp::is_same_template_v<T,C>;

template<class G>
constexpr bool is_generator_v = core::mp::is_same_template_v<G,Generator>;

// Generic pipe operator for applying operation to a generator.
template<class G, class Op>
requires is_generator_v<G>
auto operator|(G&& g, Op&& op) {
    return op(std::forward<G>(g));
}

// Generic pipe operator for applying operation to two generators.
template<class Op, class... Gs>
requires (is_generator_v<Gs> && ...)
auto operator|(std::tuple<Gs...>&& tup, Op&& op) {
    return op(std::forward<std::tuple<Gs...>>(tup));
}

template<class G, class H>
auto operator^(G&& g, H&& h) {
    return std::tuple{std::forward<G>(g), std::forward<H>(h)};
}

template<class T>
using Gen = coro::Generator<T>;

template<class T>
coro::Generator<T> nil() {
    co_return;
}

}; // costr
