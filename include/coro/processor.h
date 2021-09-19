// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/base.h"

namespace coro
{

// (possibly recusrive) Generator using symmetric transfer.
//
template<class Output, class Input>
class Processor {
public:
    using always = std::experimental::suspend_always;
    using never = std::experimental::suspend_never;
    
    class promise_type;
    using handle_type = std::experimental::coroutine_handle<promise_type>;
    using generic_handle_type = std::experimental::coroutine_handle<>;

    class promise_type {
    public:
	promise_type() { }
	
	Processor get_return_object() noexcept {
	    return Processor(handle_type::from_promise(*this));
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
	input_awaiter yield_value(input_token) noexcept {
	    return {input_};
	}

	// If a `ready_token` is yielded, always suspend.
	always yield_value(ready_token) noexcept { return {}; }
	
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
	friend Processor;
	std::exception_ptr *exception_ = nullptr;
	std::add_pointer_t<Output> value_;
	std::add_pointer_t<Input> input_;
    };

    Processor() noexcept = default;
    Processor(const Processor& other) = delete;
    
    Processor(Processor&& other) noexcept {
	std::swap(coro_, other.coro_);
    }

    Processor& operator=(Processor& other) noexcept {
	std::swap(coro_, other.coro_);
	return *this;
    }
    
    ~Processor() {
	if (coro_)
	    coro_.destroy();
    }

    // Return false iff the processor is exhausted.
    bool sink(Input input) {
	coro_.promise().input_ = std::addressof(input);
	coro_.resume();
	return coro_ and not coro_.done();
    }

    bool done() const {
	return not coro_ or coro_.done();
    }

    // Return the next value from the processor.
    Output operator()() const {
	assert(coro_.promise().value_);
	return static_cast<Output>(*coro_.promise().value_);
    }

private:
    // This should only be called by the promise.
    Processor(handle_type coro)
	: coro_(coro)
    { }

    handle_type coro_{nullptr};
};

}; // coro
