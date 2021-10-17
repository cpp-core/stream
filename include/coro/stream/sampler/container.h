// Copyright 2021 by Mark Melton
//

#pragma once
#include <deque>
#include "coro/stream/sampler.h"
#include "core/algo/random.h"
#include "core/traits/extrema.h"
#include "core/mp/same.h"

namespace costr {

template<class T>
requires (core::mp::is_same_template_v<T, vector>
	  or core::mp::is_same_template_v<T, list>
	  or core::mp::is_same_template_v<T, std::deque>)
struct Sampler<T> {
    using G = coro::Generator<T>;
    using SizeG = coro::Generator<size_t>;
    using Value = typename T::value_type;
    using ValueRef = const Value&;
    using ValueG = coro::Generator<Value>;
    
    G operator()(SizeG g_size, ValueG g_value) const {
	auto iter_size = g_size.begin();
	auto iter_value = g_value.begin();
	while (iter_size != g_size.end()) {
	    auto count = *iter_size;
	    T container;
	    for (auto i = 0; i < count; ++i, ++iter_value)
		container.push_back(*iter_value);
	    co_yield container;
	    ++iter_size;
	}
	co_return;
    }
    
    G operator()(size_t min_size, size_t max_size, ValueRef min_value, ValueRef max_value) const {
	auto g_size = sampler<size_t>(min_size, max_size);
	auto g_elem = sampler<Value>(min_value, max_value);
	return this->operator()(std::move(g_size), std::move(g_elem));
    }
    
    G operator()(size_t min_size = 0, size_t max_size = 20) const {
	auto g_size = sampler<size_t>(min_size, max_size);
	auto g_elem = sampler<Value>();
	return this->operator()(std::move(g_size), std::move(g_elem));
    }
};

template<class T>
requires (core::mp::is_same_template_v<T, set>
	  or core::mp::is_same_template_v<T, map>)
struct Sampler<T> {
    using G = coro::Generator<T>;
    using SizeG = coro::Generator<size_t>;
    using Value = typename T::value_type;
    using ValueRef = const Value&;
    using ValueG = coro::Generator<Value>;
    
    G operator()(SizeG g_size, ValueG g_value) const {
	auto iter_size = g_size.begin();
	auto iter_value = g_value.begin();
	while (iter_size != g_size.end()) {
	    auto count = *iter_size;
	    T container;
	    for (auto i = 0; i < count; ++i, ++iter_value)
		container.insert(*iter_value);
	    co_yield container;
	    ++iter_size;
	}
	co_return;
    }
    
    G operator()(size_t min_size, size_t max_size, ValueRef min_value, ValueRef max_value) const {
	auto g_size = sampler<size_t>(min_size, max_size);
	auto g_elem = sampler<Value>(min_value, max_value);
	return this->operator()(std::move(g_size), std::move(g_elem));
    }
    
    G operator()(size_t min_size = 0, size_t max_size = 20) const {
	auto g_size = sampler<size_t>(min_size, max_size);
	auto g_elem = sampler<Value>();
	return this->operator()(std::move(g_size), std::move(g_elem));
    }
};

}; // costr
