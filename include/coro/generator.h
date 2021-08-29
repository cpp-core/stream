// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include <experimental/coroutine>
#include "core/common.h"

namespace co
{

template<class T>
class Generator {
public:
    struct promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;

    struct promise_type {
	T value_;
	std::exception_ptr exception_;

	Generator get_return_object() { return Generator(handle_type::from_promise(*this)); }
	auto initial_suspend() { return std::experimental::suspend_always(); }
	auto final_suspend() noexcept { return std::experimental::suspend_always(); }
	void unhandled_exception() { exception_ = std::current_exception(); }

	template<class From>
	auto yield_value(From&& from) {
	    value_ = std::forward<From>(from);
	    return std::experimental::suspend_always();
	}

	void return_void() { }
    };

    Generator(handle_type handle)
	: handle_(handle)
    { }

    // Generator's cannot be copied so disable copy construction and
    // assignment operations.
    Generator(const Generator& other) = delete;
    Generator& operator=(const Generator& other) = delete;
    
    Generator(Generator&& other) {
	std::swap(handle_, other.handle_);
	std::swap(vacant_, other.vacant_);
    }

    Generator& operator=(Generator&& other) {
	std::swap(handle_, other.handle_);
	std::swap(vacant_, other.vacant_);
	return *this;
    }
    
    ~Generator() {
	if (handle_) {
	    handle_.destroy();
	    handle_ = nullptr;
	}
    }

    // Return false iff the generator is exhausted (i.e. it cannot be
    // invoked to compute another element); otherwise, return true.
    explicit operator bool() {
	populate();
	return not handle_.done();
    }

    // Return the next element from the generator. If the generator is
    // exhausted, return the last element.
    T operator()() {
	populate();
	vacant_ = true;
	return std::move(handle_.promise().value_);
    }

private:
    // If the generator has not computed the next element, compute the
    // next element and cache it to be returned by the call operator.
    void populate() {
	if (vacant_) {
	    handle_();
	    if (handle_.promise().exception_)
		std::rethrow_exception(handle_.promise().exception_);
	    vacant_ = false;
	}
    }
    
    handle_type handle_{nullptr};
    bool vacant_{true};
};

}; // co
