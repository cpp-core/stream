// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "coro/stream/sampler/integral.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace coro {

// Return a generator that yields elements from the underlying generators in a uniform
// random fashion.
template<class T, size_t N>
Generator<T> choose(std::array<Generator<T>,N> arr) {
    auto gidx = sampler<int>(0, arr.size() - 1);
    size_t exhausted{0};
    while (exhausted < arr.size()) {
	auto idx = gidx.sample();
	while (arr[idx].done())
	    idx = (idx + 1) % arr.size();
	if (arr[idx].next())
	    co_yield arr[idx]();
	else
	    ++exhausted;
    }
    co_return;
}

template<Stream S, Stream... Ss>
Generator<typename std::decay_t<S>::value_type> choose(std::tuple<S, Ss...> tup) {
    auto r = sampler<int>(0, sizeof...(Ss));
    using namespace core::tp;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    while (all(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	auto idx = r.sample();
	auto value = *select_nth(iterators, idx);
	co_yield value;
	apply_nth([](auto& iter) { ++iter; }, idx, iterators);
    }
    co_return;
}

// Choose randomly from an array of generators for each element.
//
// *Returns:* **Generator<...>** A generator that yields elements from the underlying
// generators in a uniform random fashion.
//
// *sampler<int>(0, 9) + sampler<int>(10, 19) + sampler<int>(20, 29) | choose()*
inline auto choose() {
    return []<class G>(G g) {
	return choose(std::move(g));
    };
}

}; // coro
