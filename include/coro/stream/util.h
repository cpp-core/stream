// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/mp/same.h"

namespace coro {

// Evaluates to true if **G** is a **Generator<...>**
template<class G>
constexpr bool is_generator_v = core::mp::is_same_template_v<std::decay_t<G>,Generator>;

template<class T>
using Gen = Generator<T>;

// Return a generator that yields no values of type `T`.
template<class T>
Generator<T> nil() {
    co_return;
}

// The **Stream** concept defines the necessary shape for a class to interoperate as a
// stream.
template<class T>
concept Stream = requires (T t) {
    typename std::decay_t<T>::value_type;
    std::begin(t);
    std::end(t);
};

// Return the value_type of **Stream** `S`.
template<Stream S>
using stream_value_t = typename std::decay_t<S>::value_type;

}; // coro
