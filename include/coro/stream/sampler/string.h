// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/sampler.h"

namespace coro {

template<>
struct Sampler<string> {
    using G = coro::Generator<string>;
    using SizeG = coro::Generator<size_t>;
    using ElemG = coro::Generator<char>;
    
    G operator()(SizeG g_size, ElemG g_elem) const;
    G operator()(char min = 'a', char max = 'z') const;
};

namespace str {

// Return a generator of lowercase strings with size in the range (min,max).
coro::Generator<string> lower(size_t min = 0, size_t max = 20);

// Return a generator of uppercase strings with size in the range (min,max).
coro::Generator<string> upper(size_t min = 0, size_t max = 20);

// Return a generator of alpha strings with size in the range (min,max).
coro::Generator<string> alpha(size_t min = 0, size_t max = 20);

// Return a generator of alpha-numeric strings with size in the range (min,max).
coro::Generator<string> alphanum(size_t min = 0, size_t max = 20);

// Return a generator of hex strings with size in the range (min,max).
coro::Generator<string> hex(bool upper = false, size_t min = 0, size_t max = 20);

// Return a generator of random strings with size in the range (min, max).
coro::Generator<string> any(size_t min = 0, size_t max = 20);

}; // str

}; // costr
