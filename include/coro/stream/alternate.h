// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

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
