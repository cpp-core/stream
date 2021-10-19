// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/util.h"

namespace costr {

template<class T, class U>
Gen<std::pair<T, U>> zip_pair(Gen<T> first, Gen<U> second) {
    auto iter_first = first.begin();
    auto iter_second = second.begin();
    while (iter_first != first.end() and iter_second != second.end()) {
	co_yield std::make_pair(*iter_first, *iter_second);
	++iter_first;
	++iter_second;
    }
    co_return;
}

}; // costr
