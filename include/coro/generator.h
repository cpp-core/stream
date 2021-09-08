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
	using value_type = std::remove_reference_t<T>;
	using reference_type = std::conditional_t<std::is_reference_v<T>, T, T&>;
	using pointer_type = value_type*;
	
	value_type value_;
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

	void rethrow_if_exception() {
	    if (exception_)
		std::rethrow_exception(exception_);
	}
    };

    struct sentinel {};

    struct iterator {
    public:
	using iterator_category = std::input_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = typename promise_type::value_type;
	using reference = typename promise_type::reference_type;
	using pointer = typename promise_type::pointer_type;

	iterator() noexcept
	: coro_(nullptr) {
	}

	explicit iterator(handle_type coro) noexcept
	    : coro_(coro) {
	}

	friend bool operator==(const iterator& iter, sentinel) noexcept {
	    return not iter.coro_ or iter.coro_.done();
	}

	friend bool operator!=(const iterator& iter, sentinel s) noexcept {
	    return not (iter == s);
	}

	friend bool operator==(sentinel s, const iterator& iter) noexcept {
	    return iter == s;
	}

	friend bool operator!=(sentinel s, const iterator& iter) noexcept {
	    return iter != s;
	}

	iterator& operator++() {
	    coro_.resume();
	    if (coro_.done())
		coro_.promise().rethrow_if_exception();
	    return *this;
	}

	reference operator*() const noexcept {
	    return coro_.promise().value_;
	}

	pointer operator->() const noexcept {
	    return std::addressof(operator*());
	}
	    
    private:
	handle_type coro_;
    };

    Generator(handle_type handle)
	: coro_(handle)
    { }

    // Generator's cannot be copied so disable copy construction and
    // assignment operations.
    Generator(const Generator& other) = delete;
    Generator& operator=(const Generator& other) = delete;
    
    Generator(Generator&& other) {
	std::swap(coro_, other.coro_);
	std::swap(vacant_, other.vacant_);
    }

    Generator& operator=(Generator&& other) {
	std::swap(coro_, other.coro_);
	std::swap(vacant_, other.vacant_);
	return *this;
    }
    
    ~Generator() {
	if (coro_) {
	    coro_.destroy();
	    coro_ = nullptr;
	}
    }

    iterator begin() {
	if (coro_) {
	    coro_.resume();
	    if (coro_.done())
		coro_.promise().rethrow_if_exception();
	}
	return iterator(coro_);
    }

    sentinel end() {
	return sentinel{};
    }

    // Return false iff the generator is exhausted (i.e. it cannot be
    // invoked to compute another element); otherwise, return true.
    explicit operator bool() {
	populate();
	return not coro_.done();
    }

    // Return the next element from the generator. If the generator is
    // exhausted, return the last element.
    T operator()() {
	populate();
	vacant_ = true;
	return std::move(coro_.promise().value_);
    }

private:
    // If the generator has not computed the next element, compute the
    // next element and cache it to be returned by the call operator.
    void populate() {
	if (vacant_) {
	    coro_();
	    if (coro_.promise().exception_)
		std::rethrow_exception(coro_.promise().exception_);
	    vacant_ = false;
	}
    }
    
    handle_type coro_{nullptr};
    bool vacant_{true};
};

}; // co
