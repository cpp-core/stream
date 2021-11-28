// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generator.h"
#include "core/utility/fixed.h"

namespace coro {

// The **stream_traits** template class is an opt-in mechanism for
// adapting a class to meet the **Stream** requirements.
template<class T>
struct stream_traits : public std::false_type { };

template<class T, class U>
struct stream_traits<Generator<T, U>> : public std::true_type {
    using value_type = typename Generator<T,U>::value_type;
    using yield_type = typename Generator<T,U>::reference_type;
};

template<class T>
struct stream_traits<std::vector<T>> : public std::true_type {
    using value_type = T;
    using yield_type = T&&;
};

template<class T>
struct stream_traits<const std::vector<T>> : public std::true_type {
    using value_type = T;
    using yield_type = const T&;
};

template<class T>
struct stream_traits<core::Fixed<std::vector<T>>> : public std::true_type {
    using value_type = T;
    using yield_type = T&&;
};

// Evaluates to true if class `T` has a **stream_traits** specialization.
template<class T>
constexpr bool has_stream_traits = stream_traits<std::decay_t<T>>::value;

// Evaluates to the **value_type** of **Stream** `T`.
template<class T>
using stream_value_t = typename stream_traits<std::remove_reference_t<T>>::value_type;

template<class T>
using stream_yield_t = typename stream_traits<std::remove_reference_t<T>>::yield_type;

// The **Stream** concept defines the necessary shape for a class to
// interoperate as a stream.
template<class T>
concept Stream = requires (T t) {
    requires has_stream_traits<T>;
    std::begin(t);
    std::end(t);
};

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
