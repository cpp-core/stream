// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include <thread>
#include "coro/base.h"
#include "core/chrono.h"

namespace cot
{

// LowResClock provides very fast access to the current time with
// configurable resolution (typically 1ms). This provides an
// alternative to the standard system time which has high resolution
// but require upwards of tens of nanos to access.
class LowResClock {
public:
    enum class Mode { RealTime, Virtual };
    
    LowResClock(Mode mode, chron::InNanos resolution);
    ~LowResClock();

    LowResClock(const LowResClock&) = delete;
    LowResClock& operator=(const LowResClock&) = delete;

    auto mode() const { return mode_; }
    chron::InNanos resolution() const { return resolution_; }
    chron::TimeInNanos now() const { return now_; }
    chron::TimeInNanos wallclock() const { return wallclock_; }
    
    void now(chron::TimeInNanos tp) {
	assert(mode_ == Mode::Virtual);
	now_ = tp;
    }
    
private:
    Mode mode_;
    chron::InNanos resolution_;
    std::atomic<chron::TimeInNanos> wallclock_, now_;
    std::thread thread_;
    std::atomic<bool> done_{false};
};

}; // cot
