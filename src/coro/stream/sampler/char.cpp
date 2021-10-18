// Copyright (C) 2021 by Mark Melton
//

#include "coro/stream/sampler/char.h"

namespace costr::chr {

Gen<char> lower() {
    return sampler<char>('a', 'z');
}

Gen<char> upper() {
    return sampler<char>('A', 'Z');
}

Gen<char> alpha() {
    size_t count = 52;
    auto g = sampler<int>(0, count - 1);
    while (true) {
	auto idx = g.sample();
	if (idx < 26) co_yield 'a' + idx;
	else co_yield 'A' + (idx - 26);
    }
    co_return;
}

Gen<char> alphanum() {
    size_t count = 62;
    auto g = sampler<int>(0, count - 1);
    while (true) {
	auto idx = g.sample();
	if (idx < 26) co_yield 'a' + idx;
	else if (idx < 52) co_yield 'A' + (idx - 26);
	else co_yield '0' + (idx - 52);
    }
    co_return;
}

Gen<char> hex(bool upper) {
    size_t count = 16;
    auto g = sampler<int>(0, count - 1);
    char alpha = upper ? 'A' : 'a';
    
    while (true) {
	auto idx = g.sample();
	if (idx < 10) co_yield '0' + idx;
	else co_yield alpha + (idx - 10);
    }
    co_return;
}

}; // costr::chr
