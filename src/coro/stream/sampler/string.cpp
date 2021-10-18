// Copyright (C) 2021 by Mark Melton
//

#include "coro/stream/sampler/string.h"
#include "coro/stream/sampler/char.h"

namespace costr {

Sampler<string>::G Sampler<string>::operator()(SizeG g_size, ElemG g_elem) const {
    while (true) {
	auto count = g_size.sample();
	string s;
	for (auto i = 0; i < count; ++i)
	    s.push_back(g_elem.sample());
	co_yield s;
    }
    co_return;
}
    
Sampler<string>::G Sampler<string>::operator()(char min, char max) const {
    auto g_size = sampler<size_t>(0, 20);
    auto g_elem = sampler<char>(min, max);
    return this->operator()(std::move(g_size), std::move(g_elem));
}

namespace str {

coro::Generator<string> lower(size_t min, size_t max) {
    return Sampler<string>{}(sampler<size_t>(min, max), chr::lower());
}

coro::Generator<string> upper(size_t min, size_t max) {
    return Sampler<string>{}(sampler<size_t>(min, max), chr::upper());
}

coro::Generator<string> alpha(size_t min, size_t max) {
    return Sampler<string>{}(sampler<size_t>(min, max), chr::alpha());
}

coro::Generator<string> alphanum(size_t min, size_t max) {
    return Sampler<string>{}(sampler<size_t>(min, max), chr::alphanum());
}

coro::Generator<string> hex(bool upper, size_t min, size_t max) {
    return Sampler<string>{}(sampler<size_t>(min, max), chr::hex(upper));
}

}; // str

}; // costr
