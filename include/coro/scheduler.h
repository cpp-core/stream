// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include <queue>
#include "coro/strand.h"
#include "coro/lowres_clock.h"
#include "core/chrono/timepoint.h"

namespace cot {

class Scheduler {
public:
    using Mode = LowResClock::Mode;
    
    struct StrandPriority {
	constexpr bool operator()(const Strand *l, const Strand *r) const {
	    return l->next_runtime() > r->next_runtime();
	};
    };
    using RunQueue = std::priority_queue<Strand*, std::vector<Strand*>, StrandPriority>;
    
    Scheduler(Mode mode, chron::InNanos resolution)
	: clock_(mode, resolution) {
    }

    virtual ~Scheduler() {
    }

    bool run();
    Strand *active() { return active_task_; }
    void unsuspend(Strand *s) { tasks().push(s); }
    
    void stop() { set_done(); }
    bool done() const { return done_; }

    const std::exception_ptr& eptr() const { return exception_ptr_; }

    Strand::Profiles profiles() const;
    string info() const;

    void on_setup(Strand&& strand) {
	setup_.emplace_back(Yield::Resume{}, std::move(strand));
    }
    
    template<class L> requires StrandLambda<L>
    void on_setup(L&& lambda) {
	on_setup(construct_strand(std::forward<L>(lambda)));
    }

    auto& on_loop(Yield::Code&& state, Strand&& strand) {
	loop_.emplace_back(std::move(state), std::move(strand));
	return loop_.back();
    }
    
    auto& on_loop(Strand&& strand) {
	return on_loop(Yield::Resume{}, std::forward<Strand>(strand));
    }

    template<class L> requires StrandLambda<L>
    auto& on_loop(Yield::Code&& state, L&& lambda) {
	return on_loop(std::forward<Yield::Code>(state),
		       construct_strand(std::forward<L>(lambda)));
    }
    
    template<class L> requires StrandLambda<L>
    auto& at_time(chron::TimeInNanos tp, L&& lambda) {
	return on_loop(Yield::ResumeAt{tp}, std::forward<L>(lambda));
    }

    template<class L> requires StrandLambda<L>
    auto& after_duration(chron::InNanos duration, L&& lambda) {
	return on_loop(Yield::ResumeAfter{duration}, std::forward<L>(lambda));
    }

    template<class L> requires StrandLambda<L>
    auto& on_loop(L&& lambda) {
	return on_loop(construct_strand(std::forward<L>(lambda)));
    }

    void on_tear_down(Strand&& strand) {
	tear_down_.emplace_back(Yield::Resume{}, std::move(strand));
    }
    
    template<class L> requires StrandLambda<L>
    void on_tear_down(L&& lambda) {
	on_tear_down(construct_strand(std::forward<L>(lambda)));
    }

    const auto& clock() const { return clock_; }
    auto& clock() { return clock_; }
    
    auto now() const { return clock_.now(); }
    auto wallclock() const { return clock_.wallclock(); }
    
protected:
    const RunQueue& tasks() const { return tasks_; }
    RunQueue& tasks() { return tasks_; }

    Strand *active_task() const { return active_task_; }
    void active_task(Strand *s) { active_task_ = s; }
    
    void set_done() { done_ = true; }
    void set_eptr(std::exception_ptr ptr) { exception_ptr_ = ptr; }
    
    RunQueue tasks_;
    Strand *active_task_{nullptr};

private:
    using Lambda = std::function<Strand()>;
    using LambdaPtr = std::unique_ptr<Lambda>;
    using LambdaPtrs = std::vector<LambdaPtr>;

    template<class L>
    Strand construct_strand(L&& lambda) {
	saved_.emplace_back(std::make_unique<Lambda>(std::forward<L>(lambda)));
	return (*saved_.back())();
    }

    bool pre_run_group(Strands& strands);
    virtual bool run_group(Strands& strands) = 0;
    
    bool done_;
    std::exception_ptr exception_ptr_;
    LambdaPtrs saved_;
    Strands setup_, loop_, tear_down_;
    LowResClock clock_;
};

using SchedulerPtr = std::unique_ptr<Scheduler>;

}; // cot

