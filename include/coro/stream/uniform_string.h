// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/uniform.h"
#include "core/algo/random.h"

namespace costr {

template<>
struct Uniform<string> {
    using G = coro::Generator<string>;
    
    G operator()(char min = 'a', char max = 'z') const {
	std::uniform_int_distribution<int> size(0, 20);
	std::uniform_int_distribution<int> elem(min, max);
	while (true) {
	    auto count = size(core::rng());
	    string s;
	    for (auto i = 0; i < count; ++i)
		s.push_back(elem(core::rng()));
	    co_yield s;
	}
	co_return;
    }
};

}; // costr
