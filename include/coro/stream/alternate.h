// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"
#include "core/tuple/to_array.h"

namespace coro {

// Return a generator that yields elements from the underlying generators in a round-robin
// fashion starting with the first elmenent from the first generator followed by the first
// element from the second generator, etc.
template<class T, size_t N>
Generator<T> alternate(std::array<Generator<T>,N> arr) {
    bool done{false};
    while (not done) {
	done = true;
	for (auto& g : arr) {
	    if (not g.done() and g.next()) {
		co_yield g();
		done = false;
	    }
	}
    }
    co_return;
}

template<Stream S, Stream... Ss>
Generator<typename std::decay_t<S>::value_type> alternate(std::tuple<S, Ss...> tup) {
    using namespace core::tp;
    auto iterators = mapply([](auto& g) { return g.begin(); }, tup);
    auto end_iters = mapply([](auto& g) { return g.end(); }, tup);
    while (any(map_n([](auto& iter, auto& end) { return iter != end; }, iterators, end_iters))) {
	auto values = map_n([](auto& iter, auto& end) {
	    if (iter != end) return std::optional{*iter};
	    else return std::optional<decltype(*iter)>{};
	}, iterators, end_iters);
	auto arr = to_array(values);
	for (auto i = 0; i < std::tuple_size<decltype(values)>(); ++i)
	    if (arr[i])
		co_yield arr[i].value();
	apply_n([](auto& iter, auto& end) { if (iter != end) ++iter; }, iterators, end_iters);
    }
    co_return;
}

// Alternate elements from an array of generators.
//
// *Returns:* **Generator<...>** A generator that yields elements from the underlying
// generators starting with the first elmenent from the first generator followed by the
// first element from the second generator, etc.
//
// *sampler<int>(0, 9) + sampler<int>(10, 19) + sampler<int>(20, 29) | alternate()*
inline auto alternate() {
    return []<class G>(G g) {
	return alternate(std::move(g));
    };
}

}; // coro
