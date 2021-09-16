// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/traits/extrema.h"

namespace cogen {

template<class T>
requires std::is_integral_v<T>
coro::Generator<T> uniform(T min = core::extrema<T>::min(), T max = core::extrema<T>::max());

}; // cogen
