// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/scheduler.h"

namespace cot {

class RealScheduler : public Scheduler {
public:
    RealScheduler(chron::InNanos resolution = 1ms)
	: Scheduler(Mode::RealTime, resolution) {
    }

    virtual ~RealScheduler() {
    }

    virtual bool run_group(Strands& strands) override;
};

}; // cot

