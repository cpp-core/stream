// Copyright (C) 2021 by Mark Melton
//

#include <thread>
#include "coro/scheduler.h"
#include "core/match.h"

namespace cot
{

bool Scheduler::run() {
    // return pre_run_group(setup_) and
    // 	pre_run_group(loop_) and
    // 	pre_run_group(tear_down_);
    return pre_run_group(loop_);
}

bool Scheduler::pre_run_group(Strands& strands) {
    done_ = false;
    return run_group(strands);
}

Strand::Profiles Scheduler::profiles() const {
    Strand::Profiles p;
    for (const auto& strand : loop_)
	p.push_back(strand.profile());
    return p;
}

string Scheduler::info() const {
    std::stringstream ss;
    size_t total_calls{0};
    chron::InNanos total_time{0};
    for (const auto& strand : loop_) {
	auto nc = strand.profile().calls;
	auto ns = strand.profile().time;

	total_calls += nc;
	total_time += ns;
	ss << "r    : " << nc << " " << ns << endl;
    }
    if (loop_.size() > 0)
	ss << "total: " << total_calls << " " << total_time << endl;
    return ss.str();
}

}; // cot

