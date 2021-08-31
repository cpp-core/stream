// Copyright (C) 2021 by Mark Melton
//

#include <thread>
#include <queue>
#include "coro/real_scheduler.h"
#include "core/match.h"

namespace cot {

static auto find_timepoint(Strand *s, chron::TimeInNanos tp) {
    return core::match
	(s->state(),
	 [&](const Yield::ResumeAt& state) { return state.tp; },
	 [&](const Yield::ResumeAfter& state) { return tp + state.duration; },
	 [&](const Yield::Resume& state) { return tp; },
	 [&](const auto& state) { 
	     throw core::runtime_error("Invalid initial runnable state: {}", state);
	     return chron::TimeInNanos::max();
	 });
}

bool RealScheduler::run_group(Strands& strands) {
    set_now_realtime();
    for (auto& s : strands) {
	if (not s.done() and not std::holds_alternative<Yield::Suspend>(s.state())) {
	    s.next_runtime() = find_timepoint(&s, now());
	    tasks().push(&s);
	}
    }

    set_now_realtime();
    while (not tasks().empty()) {
	auto start_tp = now();
	auto s = tasks().top();
	tasks().pop();

	active_task(s);
	s->coro().resume();
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
			s->next_runtime() = s->last_runtime();
			tasks().push(s);
		    },
		    [&](const Yield::ResumeAfter& state) {
			s->next_runtime() = s->last_runtime() + state.duration;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeAt& state) {
			s->next_runtime() = state.tp;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeOnSocket& state) {
			s->next_runtime() = s->last_runtime() + 10ms;
			tasks().push(s);
		    },
		    [&](const Yield::Shutdown&) {
			set_done();
		    },
		    [&](const Yield::Suspend&) {
			s->next_runtime() = s->last_runtime();
		    },
		    [&](const Yield::Terminate&) {
			set_done();
		    });

	set_now_realtime();
	s->update(start_tp, now());
	
	if (std::holds_alternative<Yield::Exception>(s->state()) or
	    std::holds_alternative<Yield::Terminate>(s->state()))
	    break;
    }
    
    return not eptr();
}

}; // cot

