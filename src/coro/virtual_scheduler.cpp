// Copyright (C) 2021 by Mark Melton
//

#include <queue>
#include "coro/virtual_scheduler.h"
#include "core/match.h"

namespace cot {

auto find_timepoint(Strand *s, chron::TimeInNanos vtp, chron::TimeInNanos rtp) {
    return core::match
	(s->state(),
	 [&](const Yield::ResumeAt& state) {
	    return std::make_pair(true, state.tp);
	},
	 [&](const Yield::ResumeAfter& state) {
	     return std::make_pair(true, vtp + state.duration);
	 },
	 [&](const Yield::ResumeAfterReal& state) {
	     return std::make_pair(false, rtp + state.duration);
	 },
	 [&](const Yield::Resume& state) {
	     return std::make_pair(true, vtp);
	 },
	 [&](const auto& state) {
	     throw core::runtime_error("Invalid initial state: {}", state);
	     return std::make_pair(true, chron::TimeInNanos::max());
	 });
}

bool VirtualScheduler::run_group(Strands& strands) {
    for (auto& s : strands) {
	if (not s.done() and not std::holds_alternative<Yield::Suspend>(s.state())) {
	    auto [virt, tp] = find_timepoint(&s, virtual_now(), now());
	    s.next_runtime() = tp;
	    if (virt) tasks().push(&s);
	    else rtasks().push(&s);
	}
    }

    while (not done() and (not tasks().empty() or not rtasks().empty())) {
	Strand *s{nullptr};
	
	if (not rtasks().empty() and rtasks().top()->next_runtime() <= now()) {
	    s = rtasks().top();
	    rtasks().pop();
	}
	else if (not tasks().empty()) {
	    s = tasks().top();
	    tasks().pop();
	}
	else {
	    auto delta = rtasks().top()->next_runtime() - now();
	    std::this_thread::sleep_for(delta);
	    continue;
	}

	auto tp = s->next_runtime();
	clock().virtual_now(tp);

	active_task(s);
	auto start_tp = now();
	s->coro().resume();
	s->update(start_tp, now());
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
			s->next_runtime() = virtual_now() + 1ns;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeAfter& state) {
			s->next_runtime() = virtual_now() + state.duration;
			tasks().push(s);
		    },
		    [&](const Yield::ResumeAfterReal& state) {
			s->next_runtime() = now() + state.duration;
			rtasks().push(s);
		    },
		    [&](const Yield::ResumeAt& state) {
			s->next_runtime() = state.tp;
			tasks().push(s);
		    },
		    [&](const Yield::Shutdown&) {
			set_done();
		    },
		    [&](const Yield::Suspend&) {
			s->next_runtime() = virtual_now() + 1ns;
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

