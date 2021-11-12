// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/adapt.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

namespace coro {

template<class T, class... Ts>
Gen<std::tuple<T,Ts...>> zip(std::tuple<Generator<T>,Generator<Ts>...> tup) {
    using core::tp::apply, core::tp::map, core::tp::mapply, core::tp::map_n, core::tp::all;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    while (all(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	co_yield map([](auto& iter) { return *iter; }, iterators);
	apply([](auto& iter) { ++iter; }, iterators);
    }
    co_return;
}

// Zip the elements from the preceeding tuple of generators in a pipeline.
//
// *Returns:* `Generator<std::tuple<...>>` A generator that yields **std::tuple**'s
// containing an element fromeach of the underlying generators. As many tuples will be
// yielded as the least number of elements yielded from an underlying generator.
//
// Usage: *sampler<int>(0,9) sampler<int>(10,19) | zip()*
inline auto zip() {
    return []<class T>(T tup) {
	return zip(std::move(tup));
    };
}

// Zip the elements from the given containers.
//
// *template param:* `C` Container that can be iterated.
// *template params:* `Cs` Containers that can be iterated.
// *param:* `c` First container to zip.
// *params:* `cs` Remaining containers to zip.
//
// *Returns:* `Generator<Cstd::tuple<C::value_type,Cs::value_type...>>` A generator that yields
// **std::tuple**'s containing an element from each of the underlying containers. As many
// tuples will be yieled as the size of the smallest container.
template<class C, class... Cs>
auto zip(const C& c, const Cs&... cs) {
    return zip(std::make_tuple(::coro::adapt(c), ::coro::adapt(cs)...));
}

}; // coro
