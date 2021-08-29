// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/strand.h"

namespace cot
{

class PeriodicallyYield {
public:
    PeriodicallyYield(chron::InNanos period)
	: period_(period)
	, next_(chron::nanopoint_from_now() + period_) {
    }
    
    bool await_ready() noexcept {
	auto tp = chron::nanopoint_from_now();
	if (tp < next_)
	    return true;
	next_ = tp + period_;
	return false;
    }
    
    void await_suspend(Strand::Handle coro) {
	coro.promise().state_ = Yield::Resume{};
    }
    
    auto await_resume() const noexcept {
	return next_;
    }
    
private:
    chron::InNanos period_;
    chron::TimeInNanos next_;
};

}; // cot
