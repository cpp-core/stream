// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/associative_decl.h"
#include "coro/generate/decl.h"

namespace cogen {

template<class T, class U>
requires AssociativeContainer<T>
Gen<T> uniform(Gen<size_t> g_size, Gen<U> g_elem) {
    auto iter_elem = g_elem.begin();
    auto iter_size = g_size.begin();
	
    while (g_size.next()) {
	T container;
	auto count = g_size();
	for (auto i = 0; i < count; ++i, ++iter_elem)
	    container.insert(*iter_elem);
	co_yield container;
    }
    co_return;
}

}; // cogen
