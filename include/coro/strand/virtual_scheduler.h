// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/strand/scheduler.h"

namespace coros {

class VirtualScheduler : public Scheduler {
public:
    VirtualScheduler(chron::TimePoint start = chron::TimePoint{},
		     chron::TimePoint end = chron::TimePoint::max(),
		     chron::nanos resolution = chron::millis{1})
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

