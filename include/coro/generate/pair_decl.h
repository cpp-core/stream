// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/util.h"
#include "core/traits/extrema.h"

namespace cogen {

template<class T, class U = typename T::first_type, class V = typename T::second_type>
requires std::is_same_v<T, std::pair<U,V>>
coro::Generator<T> uniform(coro::Generator<U> g_first = nil<U>(),
			   coro::Generator<V> g_second = nil<V>());

template<class T, class U, class V>
requires std::is_same_v<T, std::pair<U,V>>
coro::Generator<T> uniform(const U& first_min = core::extrema<U>::min(),
			   const U& first_max = core::extrema<U>::max(),
			   const V& second_min = core::extrema<V>::min(),
			   const V& second_max = core::extrema<V>::max());

}; // cogen