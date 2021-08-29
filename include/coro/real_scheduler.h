// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/scheduler.h"

namespace cot {

class RealScheduler : public Scheduler {
public:
    RealScheduler(bool debug = false)
	: Scheduler(debug) {
    }

    virtual ~RealScheduler() {
    }

    virtual bool run_group(Strands& strands) override;
};

}; // cot

