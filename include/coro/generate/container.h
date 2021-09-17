// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/container_decl.h"
#include "coro/generate/decl.h"

namespace cogen {

template<class T, class U>
requires Container<T>
Gen<T> uniform(size_t min_count, size_t max_count, const U& min, const U& max) {
    return uniform<T>(uniform(min_count, max_count), uniform(min, max));
}

template<class T, class U>
requires Container<T>
Gen<T> uniform(size_t min_count, size_t max_count, Gen<U>&& g_elem) {
    return uniform<T>(uniform(min_count, max_count), std::forward<Gen<U>>(g_elem));
}

template<class T, class U>
requires Container<T>
Gen<T> uniform(Gen<U>&& g_elem) {
    return uniform<T>(uniform<size_t>(0, 20), std::forward<Gen<U>>(g_elem));
}

template<class T, class U>
requires Container<T>
Gen<T> uniform() {
    return uniform<T>(uniform<size_t>(0, 20), uniform<U>());
}

}; // cogen
