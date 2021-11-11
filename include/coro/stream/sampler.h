// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "coro/stream/util.h"

namespace coro {

template<class T>
struct Sampler;

template<class T, class... Args>
Generator<T> sampler(Args&&... args) {
    return Sampler<T>{}(std::forward<Args>(args)...);
}

}; // costr
