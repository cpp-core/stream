// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include <experimental/coroutine>
#include "core/common.h"

namespace coro
{

// (possibly recusrive) Generator using symmetric transfer.
//
template<class Reference, class Value = std::remove_cvref_t<Reference>>
class Generator {
public:
    using always = std::experimental::suspend_always;
    using never = std::experimental::suspend_never;
    
    class promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;
    using generic_handle_type = std::experimental::coroutine_handle<>;

    class promise_type {
    public:
	promise_type() : root_or_leaf_(this) { }
	
	Generator get_return_object() noexcept {
	    return Generator(handle_type::from_promise(*this));
	}
	
	void unhandled_exception() {
	    if (exception_ == nullptr)
		throw;
	    *exception_ = std::current_exception();
	}

	// co_return is a noop.
	void return_void() { }

	// Always suspend initially.
	always initial_suspend() { return {}; }

	// final_awaiter is returned when the generator is
	// exhausted. If this is an inner generator, symmetric
	// transfer is used to return control back to the parent
	// generator. Otherwise, control is returned to the caller.
	struct final_awaiter {
	    // We always suspend.
	    bool await_ready() noexcept { return false; }

	    // If there is a parent generator, use symmetic transfer
	    // to invoke the parent coroutine; otherwise, return to
	    // the caller.
	    generic_handle_type await_suspend(handle_type coro) noexcept {
		auto& promise = coro.promise();
		auto parent = coro.promise().parent_;
		if (parent) {
		    // Keep the root pointing to the leaf (most
		    // nested) promise which the parent is about to
		    // become as we pop-up one level of "recursion"
		    // using symmetric transfer.
		    promise.root_or_leaf_->root_or_leaf_ = parent;
		    return handle_type::from_promise(*parent);
		}
		return std::experimental::noop_coroutine();
	    }
	    void await_resume() noexcept { }
	};

	// Return the final_awaiter.
	final_awaiter final_suspend() noexcept { return {}; }

	// Record the address of the yieled value.
	always yield_value(Reference&& v) noexcept {
	    root_or_leaf_->value_ = std::addressof(v);
	    return {};
	}

	// Record the address of the yielded value.
	always yield_value(Reference& v) noexcept {
	    root_or_leaf_->value_ = std::addressof(v);
	    return {};
	}

	// yield_sequence_awaiter is returned when a Generator is
	// yielded from within a Generator. 
	struct yield_sequence_awaiter {
	    Generator generator_;
	    std::exception_ptr exception_;

	    // Taking ownership of the nested generator ensures frames
	    // are destroyed in reverse order of creation.
	    explicit yield_sequence_awaiter(Generator&& generator) noexcept
		: generator_(std::move(generator)) {
	    }

	    // If the nested generator is exhausted, we skip
	    // suspending and continue with the containing generator
	    // yielding no values.
	    bool await_ready() noexcept {
		return not generator_.coro_;
	    }

	    // If the nested generator is non-empty, we do the
	    // bookkeeping necessary to return to this generator later
	    // and use symmetric transfer to invoke the inner
	    // generator coroutine.
	    generic_handle_type await_suspend(handle_type coro) noexcept {
		auto& current = coro.promise();
		auto& nested = generator_.coro_.promise();
		auto& root = current.root_or_leaf_;

		// The leaf (most nested) promise always points to the
		// root promise.
		nested.root_or_leaf_ = root;
		
		// The root promise always points to the leaf (most
		// nested) promise.
		root->root_or_leaf_ = &nested;

		// Parent tracks the immediate parent promise.
		nested.parent_ = &current;
		nested.exception_ = &exception_;

		return generator_.coro_;
	    }

	    // If there was an exception, throw it.
	    void await_resume() {
		if (exception_)
		    std::rethrow_exception(std::move(exception_));
	    }
	};

	// Yielding a generator. We transfer ownership to the awaiter
	// so that coroutine lifetimes are appropriately nested.
	yield_sequence_awaiter yield_value(Generator&& generator) noexcept {
	    return yield_sequence_awaiter{std::move(generator)};
	}

	// Resume the active coroutine.
	void resume() {
	    handle_type::from_promise(*root_or_leaf_).resume();
	}

	// Disable use of co_await within generators.
	void await_transform() = delete;
	
    private:
	friend Generator;
	
	// Pointer to the promise for either the root generator (if
	// this is a leaf generator) or the current leaf generator (if
	// this is not a leaf generator). If this is both the root and
	// leaf generator (i.e. non-recursive), then it points to
	// `this` which is both.
	promise_type *root_or_leaf_;
	// Pointer to the promise for the parent generator, if any.
	promise_type *parent_ = nullptr;
	std::exception_ptr *exception_ = nullptr;
	std::add_pointer_t<Reference> value_;
    };

    struct sentinel {};

    struct iterator {
    public:
	using iterator_category = std::input_iterator_tag;
	using difference_type = std::ptrdiff_t;
	using value_type = Value;
	using reference = Reference;
	using pointer = std::add_pointer_t<Reference>;

	iterator() noexcept = default;
	
	iterator(const iterator&) = delete;

	iterator(iterator&& other) {
	    std::swap(coro_, other.coro_);
	}

	iterator& operator=(iterator&& other) {
	    std::swap(coro_, other.coro_);
	    return *this;
	}

	~iterator() { }

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
	    coro_.promise().resume();
	    return *this;
	}

	void operator++(int) {
	    operator++();
	}

	reference operator*() const noexcept {
	    return static_cast<reference>(*coro_.promise().value_);
	}

	pointer operator->() const noexcept
	    requires std::is_reference_v<reference> {
	    return std::addressof(operator*());
	}
	    
    private:
	friend Generator;
	
	explicit iterator(handle_type coro) noexcept
	    : coro_(coro) {
	}
	    
	handle_type coro_;
    };

    Generator() noexcept = default;
    Generator(const Generator& other) = delete;
    
    Generator(Generator&& other) noexcept {
	std::swap(coro_, other.coro_);
    }

    Generator& operator=(Generator& other) noexcept {
	std::swap(coro_, other.coro_);
	return *this;
    }
    
    ~Generator() {
	if (coro_)
	    coro_.destroy();
    }

    // Return false iff the generator is exhausted (i.e. there are no
    // more values to be yielded).
    bool next() {
	coro_.promise().resume();
	return coro_ and not coro_.done();
    }

    bool done() const {
	return not coro_ or coro_.done();
    }

    // Return the next value from the generator.
    Reference operator()() const {
	return static_cast<Reference>(*coro_.promise().value_);
    }

    // Return the begin iterator resuming the coroutine which will
    // populate the promise with the first value.
    iterator begin() {
	if (coro_) 
	    coro_.resume();
	return iterator{coro_};
    }

    // Return the end iterator.
    sentinel end() {
	return {};
    }

    // Return the next value from the generator unconditionally. Throw
    // an exception if the generator is exhausted.
    Reference sample() {
	if (not coro_ or coro_.done())
	    throw std::runtime_error("generator is exhausted");
	coro_.resume();
	return static_cast<Reference>(*coro_.promise().value_);
    }

private:
    // This should only be called by the promise.
    Generator(handle_type coro)
	: coro_(coro)
    { }

    handle_type coro_{nullptr};
};

}; // coro
