// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/generate/util.h"

namespace cogen {

template<class T, class U>
Gen<std::pair<T, U>> zip_pair(Gen<T> first, Gen<U> second) {
    auto iter_first = first.begin();
    auto iter_second = second.begin();
    while (true) {
	co_yield std::make_pair(*iter_first, *iter_second);
	++iter_first;
	++iter_second;
    }
    co_return;
}

}; // cogen
