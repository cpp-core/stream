// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/sequence_decl.h"
#include "coro/stream/decl.h"

namespace costr {

template<class T, class U>
requires SequenceContainer<T>
Gen<T> sample(Gen<size_t> g_size, Gen<U> g_elem) {
    auto iter_size = g_size.begin();
    auto iter_elem = g_elem.begin();
	
    while (true) {
	auto count = *iter_size;
	++iter_size;
	T container;
	for (auto i = 0; i < count; ++i, ++iter_elem)
	    container.push_back(*iter_elem);
	co_yield container;
    }
    co_return;
}

}; // costr
