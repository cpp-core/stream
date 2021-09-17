// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/util.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

namespace cogen {

template<class T, class... Ts>
Gen<std::tuple<T,Ts...>> zip(Gen<T>&& g, Gen<Ts>&&... gs) {
    auto iters = std::make_tuple(g.begin(), gs.begin()...);
    while (true) {
	co_yield core::tp::mapply([](auto& iter) { return *iter; }, iters);
	core::tp::apply([](auto& iter) { ++iter; }, iters);
    }
    co_return;
}

}; // cogen
