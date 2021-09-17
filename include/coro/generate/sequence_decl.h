// Copyright 2021 by Mark Melton
//

#pragma once
#include <deque>
#include <list>
#include <vector>
#include "coro/generate/util.h"

namespace cogen {

template<class T>
concept SequenceContainer = is_kind<T, std::vector> ||
    is_kind<T, std::list> ||
    is_kind<T, std::deque>;

template<class T, class U = typename T::value_type>
requires SequenceContainer<T>
Gen<T> sample(Gen<size_t> g_size, Gen<U> g_elem);

}; // cogen
