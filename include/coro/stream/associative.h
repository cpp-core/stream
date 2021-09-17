// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/associative_decl.h"
#include "coro/stream/decl.h"

namespace costr {

template<class T, class U>
requires AssociativeContainer<T>
Gen<T> sample(Gen<size_t> size, Gen<U> src) {
    auto iter_elem = src.begin();
    auto iter_size = size.begin();
	
    while (size.next()) {
	T container;
	auto count = size();
	for (auto i = 0; i < count; ++i, ++iter_elem)
	    container.insert(*iter_elem);
	co_yield container;
    }
    co_return;
}

}; // costr
