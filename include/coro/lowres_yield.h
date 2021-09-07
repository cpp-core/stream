// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/strand.h"
#include "coro/lowres_clock.h"

namespace cot
{

class LowResYield {
public:
    LowResYield(const LowResClock& clock, chron::InNanos period)
	: clock_(clock)
	, period_(period)
	, next_(clock_.rnow() + period_) {
    }

    bool await_ready() noexcept {
	auto tp = clock_.rnow();
	if (tp < next_)
	    return true;
	next_ = tp + period_;
	return false;
    }

    void await_suspend(Strand::Handle coro) {
	coro.promise().state_ = Yield::Resume{};
    }

    void await_resume() const noexcept {
    }
    
private:
    const LowResClock& clock_;
    chron::InNanos period_;
    chron::TimeInNanos next_;
};

}; // cot
