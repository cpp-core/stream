// Copyright 2021 by Mark Melton
//

#pragma once
#include <set>
#include <map>
#include "coro/stream/util.h"

namespace costr {

template<class T>
concept AssociativeContainer = is_kind<T, std::set> || 
    is_kind<T, std::map>;

template<class T, class U = typename T::value_type>
requires AssociativeContainer<T>
Gen<T> sample(Gen<size_t> g_size, Gen<U> g_elem);

}; // costr
