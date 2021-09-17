// Copyright 2021 by Mark Melton
//

#pragma once
#include <deque>
#include "coro/generate/integral_decl.h"
#include "coro/generate/util.h"

namespace cogen {

template<class T>
concept ContainerPushBack = is_kind<T, std::vector> ||
    is_kind<T, std::list> ||
    is_kind<T, std::deque>;

template<class T, class U = typename T::value_type>
requires ContainerPushBack<T>
Gen<T> uniform(Gen<size_t> g_size, Gen<U> g_elem);

template<class T, class U = typename T::value_type>
requires ContainerPushBack<T>
Gen<T> uniform(size_t min_count, size_t max_count, const U& min, const U& max);

template<class T, class U = typename T::value_type>
requires ContainerPushBack<T>
Gen<T> uniform(size_t min_count, size_t max_count, Gen<U>&& g_elem);

template<class T, class U = typename T::value_type>
requires ContainerPushBack<T>
Gen<T> uniform(Gen<U>&& g_elem);

template<class T, class U = typename T::value_type>
requires ContainerPushBack<T>
Gen<T> uniform();

}; // cogen
