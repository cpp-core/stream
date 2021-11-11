// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/adapt.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

namespace coro {

// Return a generator that yields tuples. The generator zips together
// the elements yielded by the given generators and yields each set of
// elements as a tuple.
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

// Return a generator that zips together the elements of the
// underlying containers and yields each set of elements as a tuple.
template<class C, class... Cs>
auto zip(const C& c, const Cs&... cs) {
    return zip(::coro::adapt(c), ::coro::adapt(cs)...);
}

// Return a function that accepts a tuple of generators **Gs** and
// returns a new `zip` generator. The `zip` generator yields tuples of
// values zipped together from the values yielded from the underlying
// generators **Gs**.
auto zip() {
    return [=]<class T, class I = core::tp::make_tuple_index<T>>(T&& tup) {
	return core::mp::invoke_with_pack(I{}, [&](auto... Is) {
	    return zip(std::move(std::get<Is>(tup))...);
	});
    };
}

}; // coro
