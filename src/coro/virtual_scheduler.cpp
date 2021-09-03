// Copyright (C) 2021 by Mark Melton
//

#include <queue>
#include "coro/virtual_scheduler.h"
#include "core/match.h"

namespace cot {

auto find_timepoint(Strand *s) {
    return core::match
	(s->state(),
	 [&](const Yield::ResumeAt& state) { return state.tp; },
	 [&](const Yield::ResumeAfter& state) { return chron::TimeInNanos{} + state.duration; },
	 [&](const Yield::Resume& state) { return chron::TimeInNanos{}; },
	 [&](const auto& state) {
	     throw core::runtime_error("Invalid initial state: {}", state);
	     return chron::TimeInNanos::max();
	 });
}

bool VirtualScheduler::run_group(Strands& strands) {
    for (auto& s : strands) {
	if (not s.done() and not std::holds_alternative<Yield::Suspend>(s.state())) {
	    s.next_runtime() = find_timepoint(&s);
	    tasks().push(&s);
	}
    }

    while (not tasks().empty() and not done()) {
	auto s = tasks().top();
	tasks().pop();

	auto tp = s->next_runtime();
	clock().now(tp);

	active_task(s);
	auto start_tp = wallclock();
	s->coro().resume();
	s->update(start_tp, wallclock());
	active_task(nullptr);
    
	core::match(s->state(),
		    [&](const Yield::Exception& state) {
			set_done();
			set_eptr(state.eptr);
		    },
		    [&](const Yield::Finished&) {
			s->next_runtime() = chron::TimeInNanos::max();
		    },
		    [&](const Yield::Resume&) {
			s->next_runtime() = now() + 1ns;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeAfter& state) {
			s->next_runtime() = now() + state.duration;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeAt& state) {
			s->next_runtime() = state.tp;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeOnSocket& state) {
			s->next_runtime() = now() + 10ms;
			tasks().push(s);
		    },
		    [&](const Yield::Shutdown&) {
			set_done();
		    },
		    [&](const Yield::Suspend&) {
			s->next_runtime() = now() + 1ns;
		    },
		    [&](const Yield::Terminate&) {
			set_done();
		    });

	if (std::holds_alternative<Yield::Exception>(s->state()) or
	    std::holds_alternative<Yield::Terminate>(s->state()))
	    break;
    }

    return not eptr();
}

}; // cot

