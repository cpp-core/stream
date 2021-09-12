// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/strand/strand.h"

namespace coros
{

class YieldEvery {
public:
    YieldEvery(size_t count)
	: every_(count)
	, count_(0)
	, next_(count) {
    }
    
    bool await_ready() noexcept {
	++count_;
	if (count_ < next_)
	    return true;
	next_ += every_;
	return false;
    }
    
    void await_suspend(Strand::Handle coro) {
	coro.promise().state_ = Yield::Resume{};
    }
    
    void await_resume() const noexcept {
    }
    
private:
    size_t every_, count_, next_;
};

}; // coros
