// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/mp/same.h"

namespace coro {

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

// Requires that `T` has correpsonding free function `getline`.
template<class T>
concept Readable = requires (T a, std::string s) {
    getline(a, s);
};

// Requires that `T` has member method `write`.
template<class T>
concept Writeable = requires (T a, std::string s) {
    a.write(s.data(), s.size());
};

}; // coro
