// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"

namespace cogen {

template<class T>
using Gen = coro::Generator<T>;

template<class T>
coro::Generator<T> nil() {
    co_return;
}

}; // cogen
