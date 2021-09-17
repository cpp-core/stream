// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"
#include "core/traits/extrema.h"

namespace costr {

template<class T, class U = typename T::first_type, class V = typename T::second_type>
requires is_kind<T, std::pair>
coro::Generator<T> sample(coro::Generator<U> first, coro::Generator<V> g_second);

template<class T, class U = typename T::first_type, class V = typename T::second_type>
requires is_kind<T, std::pair>
coro::Generator<T> uniform(const U& first_min = core::extrema<U>::min(),
			   const U& first_max = core::extrema<U>::max(),
			   const V& second_min = core::extrema<V>::min(),
			   const V& second_max = core::extrema<V>::max());

}; // costr
