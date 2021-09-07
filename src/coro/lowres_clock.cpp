// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#include "coro/lowres_clock.h"

namespace cot
{

LowResClock::LowResClock(Mode mode, chron::InNanos resolution)
    : mode_(mode)
    , resolution_(resolution) {
    auto nanos = chron::nanopoint_from_now().time_since_epoch().count();
    auto count = resolution.count();
    
    nanos /= count;
    nanos *= count;
    real_now_.store(chron::nanopoint_from_nanos(nanos), std::memory_order_release);
    if (mode_ == Mode::RealTime)
	virtual_now_ = real_now_.load(std::memory_order_acquire);
    
    thread_ = std::thread([this]() {
	while (not done_) {
	    auto next = real_now_.load(std::memory_order_acquire) + resolution_;
	    std::this_thread::sleep_until(next);
	    real_now_ = real_now_.load(std::memory_order_acquire) + resolution_;
	    if (mode_ == Mode::RealTime)
		virtual_now_ = real_now_.load(std::memory_order_acquire);
	}
    });
}

LowResClock::~LowResClock() {
    done_ = true;
    if (thread_.joinable())
	thread_.join();
}

}; // cot
