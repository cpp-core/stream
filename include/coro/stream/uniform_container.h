// Copyright 2021 by Mark Melton
//

#pragma once
#include "coro/stream/uniform.h"
#include "core/algo/random.h"
#include "core/traits/extrema.h"
#include "core/mp/same.h"

namespace costr {

template<class T>
requires (core::mp::is_same_template_v<T, vector>
	  or core::mp::is_same_template_v<T, list>)
struct Uniform<T> {
    using Value = typename T::value_type;
    
    coro::Generator<T> operator()() const {
	std::uniform_int_distribution<size_t> dist(0, 20);
	auto elemG = Uniform<Value>{}();
	auto iter = elemG.begin();
	while (true) {
	    auto count = dist(core::rng());
	    T container;
	    for (auto i = 0; i < count; ++i, ++iter)
		container.push_back(*iter);
	    co_yield container;
	}
	co_return;
    }
};

}; // costr
