// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace costr {

template<class T>
struct Uniform;

template<class T, class... Args>
coro::Generator<T> uniform(Args&&... args) {
    return Uniform<T>{}(std::forward<Args>(args)...);
}

}; // costr
