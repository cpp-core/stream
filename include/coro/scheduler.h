// Copyright (C) 2021 by Mark Melton
//

#pragma once
#include <queue>
#include "coro/strand.h"
#include "core/chrono/timepoint.h"

namespace cot {

class Scheduler {
public:
    struct StrandPriority {
	constexpr bool operator()(const Strand *l, const Strand *r) const {
	    return l->next_runtime() > r->next_runtime();
	};
    };
    using RunQueue = std::priority_queue<Strand*, std::vector<Strand*>, StrandPriority>;
    
    Scheduler(bool debug = false)
	: debug_(debug) {
	auto tp = chron::nanopoint_from_now();
	auto ticks = chron::nanopoint_from_nanos(__rdtsc());
	offset_ = tp - ticks;
    }

    virtual ~Scheduler() {
    }

    bool run();
    Strand *active() { return active_task_; }
    void unsuspend(Strand *s) { tasks().push(s); }
    
    void stop() { set_done(); }
    bool done() const { return done_; }

    const std::exception_ptr& eptr() const { return exception_ptr_; }

    void start_debug() { debug_ = true; }
    void end_debug() { debug_ = false; }
    bool debug() const { return debug_; }
    
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

    struct AsyncPair {
	struct ReadOp {
	    ReadOp(AsyncPair *p)
		: pair(p)
	    { }
	    AsyncPair *pair{nullptr};
	    Strand *strand{nullptr};
	    bool waiting_{false};
	    chron::TimeInNanos tp_;
	    string_view data_;
	    struct Awaiter {
		bool await_ready() const noexcept { return ready_; }
		void await_suspend(cot::Strand::Handle coro) const noexcept {
		    coro.promise().state_ = cot::Yield::Suspend{};
		}
		bool await_resume() const noexcept { return ready_; }
		bool ready_;
	    };
	    Awaiter operator()(chron::TimeInNanos& tp, string_view& data) {
		if (not pair->data_ready_) {
		    waiting_ = true;
		    return Awaiter{false};
		}
		tp = tp_;
		data = data_;
		pair->writer_.strand->next_runtime() = chron::nanopoint_from_now();
		pair->scheduler_.unsuspend(pair->writer_.strand);
		pair->data_ready_ = false;
		return Awaiter{true};
	    }
	};

	struct WriteOp {
	    WriteOp(AsyncPair *p)
		: pair(p)
	    { }
	    AsyncPair *pair{nullptr};
	    Strand *strand{nullptr};
	    struct Awaiter {
		bool await_ready() const noexcept { return false; }
		void await_suspend(cot::Strand::Handle coro) const noexcept {
		    coro.promise().state_ = cot::Yield::Suspend{};
		}
		bool await_resume() const noexcept { return ready_; }
		bool ready_;
	    };
	    Awaiter operator()(chron::TimeInNanos tp, string_view data) {
		if (not pair->reader_.waiting_)
		    return Awaiter{false};
		pair->data_ready_ = true;
		pair->reader_.tp_ = tp;
		pair->reader_.data_ = data;
		pair->reader_.strand->next_runtime() = chron::nanopoint_from_now();
		pair->scheduler_.unsuspend(pair->reader_.strand);
		return Awaiter{true};
	    }
	};

	AsyncPair(Scheduler& scheduler)
	    : scheduler_(scheduler)
	    , reader_(this)
	    , writer_(this)
	    , data_ready_(false)
	{ }
	
	Scheduler& scheduler_;
	ReadOp reader_;
	WriteOp writer_;
	bool data_ready_;
    };
    using AsyncPairMap = std::map<string,AsyncPair>;
    
    AsyncPair::ReadOp *get_read_endpoint(const string& address);
    AsyncPair::WriteOp *get_write_endpoint(const string& address);
    
    auto now() const { return now_; }
    
protected:
    auto set_now_realtime() { now_ = chron::nanopoint_from_now(); }
    
    const RunQueue& tasks() const { return tasks_; }
    RunQueue& tasks() { return tasks_; }

    Strand *active_task() const { return active_task_; }
    void active_task(Strand *s) { active_task_ = s; }
    
    void set_done() { done_ = true; }
    void set_eptr(std::exception_ptr ptr) { exception_ptr_ = ptr; }
    
    chron::TimeInNanos now_;
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
    
    bool done_, debug_;
    std::exception_ptr exception_ptr_;
    LambdaPtrs saved_;
    Strands setup_, loop_, tear_down_;
    AsyncPairMap endpoints_;
    chron::InNanos offset_;
};

using SchedulerPtr = std::unique_ptr<Scheduler>;

}; // cot

