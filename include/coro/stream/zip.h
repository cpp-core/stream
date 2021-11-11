// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/adapt.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

namespace coro {

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
    return zip(adapt(c), adapt(cs)...);
}

auto zip() {
    return [=]<class G, class H>(std::tuple<G,H>&& tup) {
	return zip(std::move(std::get<0>(tup)), std::move(std::get<1>(tup)));
    };
}

}; // coro
