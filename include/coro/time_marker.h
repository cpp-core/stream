// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/strand.h"
#include "coro/scheduler.h"

namespace cot
{

class TimeMarker {
public:
    TimeMarker(chron::TimeInNanos tp, Scheduler& scheduler)
	: tp_(tp)
	, scheduler_(scheduler) {
    }
	
    bool await_ready() const noexcept {
	return false;
	// return scheduler_.try_advance_current_time(tp_);
    }
    
    void await_suspend(Strand::Handle coro) noexcept {
	coro.promise().state_ = Yield::ResumeAt{tp_};
    }
	
    void await_resume() const noexcept {
    }
	
private:
    chron::TimeInNanos tp_;
    Scheduler& scheduler_;
};

}; // cot
