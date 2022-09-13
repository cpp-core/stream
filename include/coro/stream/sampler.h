// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

template<class T>
struct Sampler;

// Return a generator that yields randomly sampled **T**'s.
template<class T, class... Args>
Generator<T> sampler(Args&&... args) {
    return Sampler<T>{}(std::forward<Args>(args)...);
}

}; // coro
