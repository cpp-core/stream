// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include <string>
#include "coro/stream/generator.h"
#include "coro/stream/detail/fixed.h"

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
    using yield_type = T&;
};

template<class T>
struct stream_traits<const std::vector<T>> : public std::true_type {
    using value_type = T;
    using yield_type = const T&;
};

template<class T>
struct stream_traits<coro::detail::Fixed<std::vector<T>>> : public std::true_type {
    using value_type = T;
    using yield_type = T&;
};

// Evaluates to true if class `T` has a **stream_traits** specialization.
template<class T>
constexpr bool has_stream_traits = stream_traits<std::decay_t<T>>::value;

// Evaluates to the **value_type** of **Stream** `T`.
template<class T>
using stream_value_t = typename stream_traits<std::remove_reference_t<T>>::value_type;

template<class T>
using stream_yield_t = typename stream_traits<std::remove_reference_t<T>>::yield_type;

namespace detail {
template<class T, bool l, bool r, bool c>
struct compatible_type_helper;

template<class T, bool c>
struct compatible_type_helper<T, false, false, c> {
    using type = std::conditional_t<c, const T, T>;
};

template<class T, bool r, bool c>
struct compatible_type_helper<T, true, r, c> {
    using type = std::conditional_t<c, const T&, T&>;
};

template<class T, bool c>
struct compatible_type_helper<T, false, true, c> {
    using type = std::conditional_t<c, const T&&, T&&>;
};

}; // detail

template<class... Ts>
struct compatible_type {
    using T = std::common_type_t<Ts...>;
    static constexpr bool l = (std::is_lvalue_reference_v<Ts> || ...);
    static constexpr bool r = (std::is_rvalue_reference_v<Ts> || ...);
    static constexpr bool c = (std::is_const_v<std::remove_reference_t<Ts>> || ...);
    using type = typename detail::compatible_type_helper<T, l, r, c>::type;
};

template<class... Ts>
using compatible_type_t = typename compatible_type<Ts...>::type;

template<class... Ss>
using streams_yield_t = compatible_type_t<stream_yield_t<Ss>...>;

// The **Stream** concept defines the necessary shape for a class to
// interoperate as a stream.
template<class T>
concept Stream = requires (T t) {
    requires has_stream_traits<T>;
    std::begin(t);
    std::end(t);
};

namespace detail {
template<class T> struct is_tuple_of_stream: std::false_type { };
template<Stream... Ss> struct is_tuple_of_stream<std::tuple<Ss...>>: std::true_type { };
}; // detail

template<class T>
concept TupleOfStream = detail::is_tuple_of_stream<T>::value;
    
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

namespace detail {

template<class T, class U, template<class...> class V>
struct is_same_template : std::false_type
{ static constexpr bool value = false; };

template<template<class...> class T, template<class...> class U, class... Ts>
struct is_same_template<T<Ts...>, U<Ts...>, U>
{
    using type = std::true_type;
    static constexpr bool value = std::is_same_v<T<Ts...>,U<Ts...>>;
};

}; // detail

template<class T, template<class...> class U>
using is_same_template_t = typename detail::is_same_template<T,T,U>::type;

template<class T, template<class...> class U>
static constexpr bool is_same_template_v = detail::is_same_template<T,T,U>::value;

}; // coro
