// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/char_decl.h"
#include "coro/stream/integral_decl.h"

namespace costr::chr {

Gen<char> uniform() {
    return costr::uniform<char>(-128, 127);
}

Gen<char> lowercase() {
    return costr::uniform('a', 'z');
}

Gen<char> uppercase() {
    return costr::uniform('A', 'Z');
}

Gen<char> alpha() {
    size_t count_az = 'z' - 'a';
    size_t count_AZ = 'Z' - 'A';
    size_t count = count_az + count_AZ;
    auto idx = costr::uniform<int>(0, count);
    auto iter_idx = idx.begin();
    while (true) {
	auto idx = *iter_idx;
	if (idx < count_az) co_yield 'a' + idx;
	else co_yield 'A' + (idx - count_az);
	++iter_idx;
    }
    co_return;
}

Gen<char> alphanum() {
    size_t count_az = 'z' - 'a';
    size_t count_AZ = 'Z' - 'A';
    size_t count_num = '9' - '0';
    size_t count = count_az + count_AZ + count_num;
    auto idx = costr::uniform<int>(0, count);
    auto iter_idx = idx.begin();
    while (true) {
	auto idx = *iter_idx;
	if (idx < count_az) co_yield 'a' + idx;
	else if (idx < count_az + count_AZ) co_yield 'A' + (idx - count_az);
	else co_yield '0' + (idx - count_az - count_AZ);
	++iter_idx;
    }
    co_return;
}

}; // costr
