// Copyright (C) 2021 by Mark Melton
//

#include <thread>
#include "coro/scheduler.h"
#include "core/match.h"

namespace cot
{

bool Scheduler::fast_forward(chron::TimeInNanos tp) {
    if (clock().mode() == Mode::RealTime)
	return true;
    if (tasks().empty() or tp < tasks().top()->next_runtime()) {
	clock().now(tp);
	return true;
    }
    return false;
}

bool Scheduler::run() {
    for (auto& f : setup_)
	f();
    bool rc = run_group(loop_);
    for (auto& f : tear_down_)
	f();
    return rc;
}

Strand::Profiles Scheduler::profiles() const {
    Strand::Profiles p;
    for (const auto& strand : loop_)
	p.push_back(strand.profile());
    return p;
}

}; // cot

