// Copyright 2021, 2022 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"

namespace coro {

template<>
struct Sampler<std::string> {
    using G = coro::Generator<std::string>;
    using SizeG = coro::Generator<size_t>;
    using ElemG = coro::Generator<char>;
    
    G operator()(SizeG g_size, ElemG g_elem) const;
    G operator()(char min = 'a', char max = 'z') const;
};

namespace str {

/// Return a generator of lowercase strings with size in the range (min,max).
coro::Generator<std::string> lower(size_t min = 0, size_t max = 20);

/// Return a generator of uppercase strings with size in the range (min,max).
coro::Generator<std::string> upper(size_t min = 0, size_t max = 20);

/// Return a generator of alpha strings with size in the range (min,max).
coro::Generator<std::string> alpha(size_t min = 0, size_t max = 20);

/// Return a generator of alpha-numeric strings with size in the range (min,max).
coro::Generator<std::string> alphanum(size_t min = 0, size_t max = 20);

/// Return a generator of binary strings with size in the range (min,max).
coro::Generator<std::string> binary(size_t min = 0, size_t max = 20);

/// Return a generator of octal strings with size in the range (min,max).
coro::Generator<std::string> octal(size_t min = 0, size_t max = 20);

/// Return a generator of decimal strings with size in the range (min,max).
coro::Generator<std::string> decimal(size_t min = 0, size_t max = 20);

/// Return a generator of hex strings with size in the range (min,max).
coro::Generator<std::string> hex(bool upper = false, size_t min = 0, size_t max = 20);

/// Return a generator of random strings with size in the range (min, max).
coro::Generator<std::string> any(size_t min = 0, size_t max = 20);

}; // str

}; // costr
