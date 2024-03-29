// Copyright (C) 2021, 2022, 2023 by Mark Melton
//

#include <string>
#include "coro/stream/sampler/char.h"

namespace coro::chr {

Generator<char> lower() {
    return sampler<char>('a', 'z');
}

Generator<char> upper() {
    return sampler<char>('A', 'Z');
}

Generator<char> alpha() {
    size_t count = 52;
    auto g = sampler<int>(0, count - 1);
    while (true) {
	auto idx = g.sample();
	if (idx < 26) co_yield 'a' + idx;
	else co_yield 'A' + (idx - 26);
    }
    co_return;
}

Generator<char> alphanum() {
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

Generator<char> binary() {
    auto g = sampler<int>(0, 1);
    while (true) {
	auto idx = g.sample();
	co_yield '0' + idx;
    }
    co_return;
}

Generator<char> octal() {
    auto g = sampler<int>(0, 7);
    while (true) {
	auto idx = g.sample();
	co_yield '0' + idx;
    }
    co_return;
}

Generator<char> decimal() {
    auto g = sampler<int>(0, 9);
    while (true) {
	auto idx = g.sample();
	co_yield '0' + idx;
    }
    co_return;
}

Generator<char> hex(bool upper) {
    auto g = sampler<int>(0, 15);
    char alpha = upper ? 'A' : 'a';
    
    while (true) {
	auto idx = g.sample();
	if (idx < 10) co_yield '0' + idx;
	else co_yield alpha + (idx - 10);
    }
    co_return;
}

}; // costr::chr
