// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/integral_decl.h"
#include "coro/generate/util.h"

namespace cogen {

template<class T, class U = typename T::value_type>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(Gen<size_t> g_size, Gen<U> g_elem);

template<class T, class U = typename T::value_type>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(size_t min_count = 0, size_t max_count = 20,
	       const U& min = core::extrema<U>::min(),
	       const U& max = core::extrema<U>::max());

template<class T, class U = typename T::value_type>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(size_t min_count, size_t max_count, Gen<U>&& g_elem);

template<class T, class U = typename T::value_type>
requires std::is_same_v<vector<U>, T>
Gen<T> uniform(Gen<U>&& g_elem);

}; // cogen
