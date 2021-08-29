// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include "coro/strand.h"
#include "core/chrono/timepoint.h"

namespace cot {

class Scheduler {
public:
    Scheduler(bool debug = false)
	: debug_(debug) {
    }

    virtual ~Scheduler() {
    }

    bool run();
    Strand *active() { return active_; }
    void unsuspend(Strand *s) { }
    
    void stop() { set_done(); }
    bool done() const { return done_; }

    const std::exception_ptr& eptr() const { return exception_ptr_; }

    void start_debug() { debug_ = true; }
    void end_debug() { debug_ = false; }
    bool debug() const { return debug_; }
    
    Strand::Profiles profiles() const;
    string info() const;

    auto current_time() const { return current_tp_; }
    auto next_time() const { return next_tp_; }
    
    bool try_advance_current_time(chron::TimeInNanos tp) {
	if (current_time() and tp < *next_time()) {
	    current_tp_ = tp;
	    return true;
	}
	return false;
    }

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

protected:
    void set_done() { done_ = true; }
    void set_eptr(std::exception_ptr ptr) { exception_ptr_ = ptr; }
    std::optional<chron::TimeInNanos> current_tp_, next_tp_;

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
    
    bool done_, debug_;
    std::exception_ptr exception_ptr_;
    chron::TimeInNanos now_;
    LambdaPtrs saved_;
    Strands setup_, loop_, tear_down_;
    Strand *active_{nullptr};
};

using SchedulerPtr = std::unique_ptr<Scheduler>;

}; // cot

