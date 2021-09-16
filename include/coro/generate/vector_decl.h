// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/integral_decl.h"
#include "coro/generate/util.h"

namespace cogen {

template<class T, class U = typename T::value_type>
requires std::is_same_v<vector<U>, T>
coro::Generator<T> uniform(coro::Generator<U> g_elem = nil<U>(),
			   coro::Generator<size_t> g_size = uniform<size_t>(0, 20));

template<class T, class U>
requires std::is_same_v<vector<U>, T>
coro::Generator<T> uniform(const U& min = core::extrema<U>::min(),
			   const U& max = core::extrema<U>::max(),
			   size_t min_count = 0, size_t max_count = 20);


}; // cogen
