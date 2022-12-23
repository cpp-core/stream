// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

template<class T>
struct LogNormalSampler;

// Return a generator that yields log-normal sampled **T**'s.
template<class T, class... Args>
Generator<T> log_normal_sampler(Args&&... args) {
    return LogNormalSampler<T>{}(std::forward<Args>(args)...);
}

}; // coro
