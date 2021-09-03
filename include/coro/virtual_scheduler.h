// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/scheduler.h"

namespace cot {

class VirtualScheduler : public Scheduler {
public:
    VirtualScheduler(chron::InNanos resolution = 1ms)
	: Scheduler(Mode::Virtual, resolution) {
    }

    virtual ~VirtualScheduler() {
    }

private:
    virtual bool run_group(Strands& strands) override;
};

}; // cot

