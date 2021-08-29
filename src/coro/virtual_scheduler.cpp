// Copyright (C) 2021 by Mark Melton
//

#include <queue>
#include "coro/virtual_scheduler.h"
#include "core/match.h"

namespace cot {

// auto find_next_timepoint(Scheduler::Runnables& runnables) {
    // Scheduler::Runnable *first_runnable{nullptr};
    // auto first_tp = chron::TimeInNanos::max(), second_tp = chron::TimeInNanos::max();
    // for (auto& runnable : runnables) {
    // 	if (runnable.done())
    // 	    continue;
    // 	auto tp = core::match
    // 	    (runnable.state(),
    // 	     [&](const Yield::ResumeAt& value) {
    // 		 return value.tp;
    // 	     },
    // 	     [&](const Yield::Resume& value) {
    // 		 return chron::TimeInNanos{};
    // 	     },
    // 	     [&](const auto& other) {
    // 		 return chron::TimeInNanos::max();
    // 	     });
    // 	if (tp < first_tp) {
    // 	    second_tp = first_tp;
    // 	    first_tp = tp;
    // 	    first_runnable = &runnable;
    // 	}
    // 	else if (tp < second_tp)
    // 	    second_tp = tp;
    // }
    // return std::make_tuple(first_runnable, first_tp, second_tp);
// }

// auto find_timepoint(Scheduler::Runnable *r) {
//     return core::match
// 	(r->state(),
// 	 [&](const Yield::ResumeAt& value) { return value.tp; },
// 	 [&](const Yield::Resume& value) { return chron::TimeInNanos{}; },
// 	 [&](const auto& value) { return chron::TimeInNanos::max(); }
// 	 );
// }

// void transform_durations(chron::TimeInNanos current_tp, Scheduler::Runnables& runnables) {
//     for (auto& r : runnables)
// 	if (std::holds_alternative<Yield::ResumeAfter>(r.state())) {
// 	    auto tp = current_tp + std::get<Yield::ResumeAfter>(r.state()).duration;
// 	    r.strand_.state() = Yield::ResumeAt{tp};
// 	    r.next_ = tp;
// 	}
// }

bool VirtualScheduler::run_group(Strands& strands) {
    // auto cmp = [](Runnable* left, Runnable* right) { return left->next_ > right->next_; };
    // std::priority_queue<Runnable*, std::vector<Runnable*>, decltype(cmp)> tasks;

    // for (auto& runnable : runnables) {
    // 	if (not runnable.strand_.done()) {
    // 	    runnable.next_ = find_timepoint(&runnable);
    // 	    tasks.push(&runnable);
    // 	}
    // }

    // current_tp_ = std::nullopt;
    // next_tp_ = std::nullopt;
    // while (not tasks.empty()) {
    // 	auto r = tasks.top();
    // 	tasks.pop();

    // 	if (r->next_ != chron::TimeInNanos{} and
    // 	    r->next_ != chron::TimeInNanos::max()) {
    // 	    if (not current_time())
    // 		transform_durations(r->next_, runnables);
    // 	    current_tp_ = r->next_;
    // 	}
    // 	if (current_time())
    // 	    next_tp_ = tasks.empty() ? chron::TimeInNanos::max() : tasks.top()->next_;

    // 	r->resume();

    // 	core::match(r->state(),
    // 		    [&](const Yield::ResumeAfter& value) {
    // 			if (current_time()) {
    // 			    auto dtp = std::get<Yield::ResumeAfter>(r->state()).duration;
    // 			    auto tp = *current_time() + dtp;
    // 			    r->strand_.state() = Yield::ResumeAt{tp};
    // 			    r->next_ = tp;
    // 			} else {
    // 			    r->next_ = chron::TimeInNanos::max();
    // 			}
    // 			tasks.push(r);
    // 		    },
    // 		    [&](const Yield::ResumeAt& v) {
    // 			r->next_ = v.tp;
    // 			tasks.push(r);
    // 		    },
    // 		    [&](const Yield::Finished&) {
    // 		    },
    // 		    [&](const Yield::Resume&) {
    // 			r->next_ = chron::TimeInNanos{};
    // 			tasks.push(r);
    // 		    },
    // 		    [&](const Yield::Shutdown&) {
    // 			set_done();
    // 		    },
    // 		    [&](const Yield::Terminate&) {
    // 		    },
    // 		    [&](const auto& other) {
    // 			r->next_ = chron::TimeInNanos::max();
    // 			tasks.push(r);
    // 		    });

    // 	if (std::holds_alternative<Yield::Terminate>(r->state()))
    // 	    break;
    // }

    return not eptr();
}

}; // cot

