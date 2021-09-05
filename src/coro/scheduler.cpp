// Copyright (C) 2021 by Mark Melton
//

#include <thread>
#include "coro/scheduler.h"
#include "core/match.h"

namespace cot
{

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

