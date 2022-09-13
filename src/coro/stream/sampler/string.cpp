// Copyright (C) 2021, 2022 by Mark Melton
//

#include <string>
#include "coro/stream/sampler/string.h"
#include "coro/stream/sampler/char.h"

namespace coro {

Sampler<std::string>::G Sampler<std::string>::operator()(SizeG g_size, ElemG g_elem) const {
    while (true) {
	auto count = g_size.sample();
	std::string s;
	for (auto i = 0; i < count; ++i)
	    s.push_back(g_elem.sample());
	co_yield s;
    }
    co_return;
}
    
Sampler<std::string>::G Sampler<std::string>::operator()(char min, char max) const {
    auto g_size = sampler<size_t>(0, 20);
    auto g_elem = sampler<char>(min, max);
    return this->operator()(std::move(g_size), std::move(g_elem));
}

namespace str {

coro::Generator<std::string> lower(size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), chr::lower());
}

coro::Generator<std::string> upper(size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), chr::upper());
}

coro::Generator<std::string> alpha(size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), chr::alpha());
}

coro::Generator<std::string> alphanum(size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), chr::alphanum());
}

coro::Generator<std::string> decimal(size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), chr::decimal());
}

coro::Generator<std::string> hex(bool upper, size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), chr::hex(upper));
}

coro::Generator<std::string> any(size_t min, size_t max) {
    return Sampler<std::string>{}(sampler<size_t>(min, max), sampler<char>());
}

}; // str

}; // costr
