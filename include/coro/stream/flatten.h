// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/type/type_name.h"

namespace coro {

// Return a container **C<T>** with all the elements from **Stream** `source`.
template<class C, Stream S>
auto flatten(S source) {
    C c;
    for (auto&& value : source)
	c.insert(c.end(), value.begin(), value.end());
    return c;
}

// Collect all the elements from a **Stream** and insert them into a container of type
// **C**.
//
// Return a function that accepts a **Stream** and returns a container **C<`T`>** with all
// the elements from the **Stream**.
template<class C>
auto flatten() {
    return []<Stream S>(S&& s) {
	return flatten<C, S>(std::forward<S>(s));
    };
}

}; // coro
