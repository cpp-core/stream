// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/strand/scheduler.h"

namespace coros {

class VirtualScheduler : public Scheduler {
public:
    VirtualScheduler(chron::TimeInNanos start = chron::TimeInNanos{},
		     chron::TimeInNanos end = chron::TimeInNanos::max(),
		     chron::InNanos resolution = 1ms)
	: Scheduler(Mode::Virtual, start, end, resolution) {
    }

    virtual ~VirtualScheduler() {
    }

    RunQueue& rtasks() { return rtasks_; }
    const RunQueue& rtasks() const { return rtasks_; }

private:
    virtual bool run_group(Strands& strands) override;
    RunQueue rtasks_;
};

}; // coros

