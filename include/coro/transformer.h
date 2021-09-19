// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/base.h"

namespace coro
{

// Transformer - consumes values of type Input optionally yielding values
// of type Output analogous to a Generator.
//
template<class Input, class Output = bool>
class Transformer {
public:
    using always = std::experimental::suspend_always;
    using never = std::experimental::suspend_never;
    
    class promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;
    using generic_handle_type = std::experimental::coroutine_handle<>;

    class promise_type {
    public:
	promise_type() { }
	
	Transformer get_return_object() noexcept {
	    return Transformer(handle_type::from_promise(*this));
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

	// Always suspend when done.
	always final_suspend() noexcept { return {}; }

	struct input_awaiter {
	    std::add_pointer_t<Input> input_;
	    // Never suspend
	    bool await_ready() const noexcept { return true; }
	    // Since we never suspend, is never called.
	    void await_suspend(handle_type coro) noexcept { }
	    // Return the input pointer.
	    auto await_resume() const noexcept { return static_cast<Input>(*input_); }
	};
	
	// If an `input_token` is yieled, return the current input via
	// the input awaiter without suspending.
	input_awaiter yield_value(input) noexcept {
	    return {input_};
	}

	// If a `ready_token` is yielded, always suspend.
	always yield_value(input_done) noexcept { return {}; }
	
	// Record the address of the yieled value and suspend.
	always yield_value(Output&& v) noexcept {
	    value_ = std::addressof(v);
	    return {};
	}

	// Record the address of the yielded value and suspend.
	always yield_value(Output& v) noexcept {
	    value_ = std::addressof(v);
	    return {};
	}

	// Disable use of co_await within generators.
	void await_transform() = delete;
	
    private:
	friend Transformer;
	std::exception_ptr *exception_ = nullptr;
	std::add_pointer_t<Output> value_;
	std::add_pointer_t<Input> input_;
    };

    Transformer() noexcept = default;
    Transformer(const Transformer& other) = delete;
    
    Transformer(Transformer&& other) noexcept {
	std::swap(coro_, other.coro_);
    }

    Transformer& operator=(Transformer& other) noexcept {
	std::swap(coro_, other.coro_);
	return *this;
    }
    
    ~Transformer() {
	if (coro_)
	    coro_.destroy();
    }

    // Consume the given <input> and return false iff the transformer is
    // exhausted.
    bool push(Input input) {
	coro_.promise().input_ = std::addressof(input);
	coro_.resume();
	return not done();
    }

    // Return true iff the transformer is exhausted.
    bool done() const {
	return not coro_ or coro_.done();
    }

    // Return the last output value yieled by the transformer.
    Output operator()() const {
	assert(coro_.promise().value_);
	return static_cast<Output>(*coro_.promise().value_);
    }

    // Return a reference to the last output value yieled by the transformer.
    Output& reference() const {
	assert(coro_.promise().value_);
	return static_cast<Output&>(*coro_.promise().value_);
    }

private:
    // This should only be called by the promise.
    Transformer(handle_type coro)
	: coro_(coro)
    { }

    handle_type coro_{nullptr};
};

}; // coro
