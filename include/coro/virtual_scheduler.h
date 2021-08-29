// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/scheduler.h"

namespace cot {

class VirtualScheduler : public Scheduler {
public:
    VirtualScheduler(bool debug = false)
	: Scheduler(debug) {
    }

    virtual ~VirtualScheduler() {
    }

private:
    virtual bool run_group(Strands& strands) override;
};

}; // cot

