// Copyright 2021 by Mark Melton
//

#pragma once
#include <deque>
#include <list>
#include <map>
#include <set>
#include <vector>
#include "coro/stream/util.h"

namespace costr {

template<class T>
concept Container =
    is_kind<T, std::deque> ||
    is_kind<T, std::list> ||
    is_kind<T, std::map> ||
    is_kind<T, std::set> ||
    is_kind<T, std::vector>;

template<class T, class U = typename T::value_type>
requires Container<T>
Gen<T> sample(size_t min_count, size_t max_count, Gen<U>&& g_elem);

template<class T, class U = typename T::value_type>
requires Container<T>
Gen<T> sample(Gen<U>&& g_elem);

template<class T, class U = typename T::value_type>
requires Container<T>
Gen<T> uniform();

template<class T, class U = typename T::value_type>
requires Container<T>
Gen<T> uniform(size_t min_count, size_t max_count, const U& min, const U& max);

}; // costr