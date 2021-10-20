// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/to_stream.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

namespace costr {

template<class T, class... Ts>
Gen<std::tuple<T,Ts...>> zip(Gen<T> g, Gen<Ts>... gs) {
    using core::tp::apply, core::tp::map, core::tp::map_n, core::tp::all;

    auto iterators = std::make_tuple(g.begin(), gs.begin()...);
    auto end_iters = std::make_tuple(g.end(), gs.end()...);
    while (all(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	co_yield map([](auto& iter) { return *iter; }, iterators);
	apply([](auto& iter) { ++iter; }, iterators);
    }
    co_return;
}

template<class C, class... Cs>
auto zip(const C& c, const Cs&... cs) {
    return zip(to_stream(c), to_stream(cs)...);
}

}; // costr
