// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/base.h"
#include "coro/yield.h"
#include "core/mp/contains.h"

namespace cot
{

class Strand {
public:
    struct Frame;
    using Handle = coro::handle<Frame>;
    
    struct Profile {
	size_t calls{0};
	chron::InNanos time{0};
    };
    using Profiles = std::vector<Profile>;
    
    struct Frame {
	Strand get_return_object() { return Strand{Handle::from_promise(*this)}; }
	coro::suspend_always initial_suspend() noexcept { return {}; }
	coro::suspend_always final_suspend() noexcept { return {}; }
	void unhandled_exception() { state_ = Yield::Exception{std::current_exception()}; }
	
	template<class T>
	requires (core::mp::contains_v<T, Yield::CoYieldTypes>)
	coro::suspend_always yield_value(T&& value) {
	    state_ = std::move(value);
	    return {};
	}

	template<class T>
	requires (core::mp::contains_v<T, Yield::CoReturnTypes>)
	void return_value(T&& value) {
	    state_ = std::move(value);
	}

	Yield::Code state_{Yield::Resume{}};
    };
    using promise_type = Frame;

     Strand(Handle handle)
	: coro_(handle)
    { }

    ~Strand() {
	if (coro_)
	    coro_.destroy();
    }

    Strand(const Strand&) = delete;
    Strand& operator=(const Strand&) = delete;

    Strand(Strand&& other) noexcept {
	std::swap(coro_, other.coro_);
    }

    Strand(Yield::Code state, Strand&& other) noexcept {
	std::swap(coro_, other.coro_);
	coro_.promise().state_ = state;
    }

    bool done() const { return coro_.done(); }
    auto id() const { return coro_.address(); }

    const auto& state() const { return coro_.promise().state_; }
    auto& state() { return coro_.promise().state_; }

    const Handle& coro() const { return coro_; }
    Handle& coro() { return coro_; }
    Frame *frame() { return &coro_.promise(); }

    const auto& last_runtime() const { return last_; }
    auto& last_runtime() { return last_; }
    
    const auto& next_runtime() const { return next_; }
    auto& next_runtime() { return next_; }
    
    const auto& profile() const { return profile_; }

    auto clock() { return ++clock_; }

    void update(chron::TimeInNanos start_tp, chron::TimeInNanos end_tp) {
	last_runtime() = end_tp;
	++profile_.calls;
	profile_.time += end_tp - start_tp;
    }

private:
    inline static size_t clock_{0};
    Handle coro_;
    chron::TimeInNanos last_;
    chron::TimeInNanos next_;
    Profile profile_;
};

using Strands = std::vector<Strand>;

template<class T>
concept StrandLambda = requires (T a) { requires std::is_same_v<decltype(a()), Strand>; };

}; // cot
