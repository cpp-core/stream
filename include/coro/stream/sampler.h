// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace coro {

template<class T>
struct Sampler;

// Return a generator that yields random **T**'s uniformly sampled.
template<class T, class... Args>
Generator<T> sampler(Args&&... args) {
    return Sampler<T>{}(std::forward<Args>(args)...);
}

// Return a generator that yields random **T's** uniformly sampled.
template<class T, class... Args>
Generator<T> uniform_sampler(Args&&... args) {
    return Sampler<T>{}(std::forward<Args>(args)...);
}

// Return a generator that yields random **T's** with a log normally sampled
// magnitude and a random sign.
template<class T, class... Args>
Generator<T> log_sampler(Args&&... args) {
    return Sampler<T>{}.log_normal(std::forward<Args>(args)...);
}

// Return a generator that yields random **T's** with a log normally
// sampled magnitude and positive sign().
template<class T, class... Args>
auto log_magnitude_sampler(Args&&... args) {
    return Sampler<T>{}.log_normal_magnitude(std::forward<Args>(args)...);
}

}; // coro
