// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/string_decl.h"
#include "coro/stream/char_decl.h"

namespace costr::str {

Gen<string> sample(Gen<size_t> size, Gen<char> src) {
    auto iter_size = size.begin();
    auto iter_src = src.begin();
    while (true) {
	auto count = *iter_size;
	++iter_size;
	string s;
	s.reserve(count);
	for (auto i = 0; i < count; ++i, ++iter_src)
	    s.push_back(*iter_src);
	co_yield s;
    }
    co_return;
}

Gen<string> uniform(Gen<size_t>&& size) {
    return sample(std::forward<Gen<size_t>>(size), costr::chr::uniform());
}

Gen<string> lowercase(Gen<size_t>&& size) {
    return sample(std::forward<Gen<size_t>>(size), costr::chr::lowercase());
}

Gen<string> uppercase(Gen<size_t>&& size) {
    return sample(std::forward<Gen<size_t>>(size), costr::chr::uppercase());
}

Gen<string> alpha(Gen<size_t>&& size) {
    return sample(std::forward<Gen<size_t>>(size), costr::chr::alpha());
}

Gen<string> alphanum(Gen<size_t>&& size) {
    return sample(std::forward<Gen<size_t>>(size), costr::chr::alphanum());
}

// Gen<string> ident(Gen<size_t> size) {
//     return sample(std::forward<Gen<size_t>>(size), costr::chr::ident());
// }

}; // costr
