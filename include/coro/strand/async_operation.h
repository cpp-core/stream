// Copyright (C) 2017, 2018, 2019, 2021 by Mark Melton
//

#pragma once
#include "coro/strand/strand.h"

namespace coros
{

template<class F>
struct AsyncOperation {
    using ReturnType = std::invoke_result_t<F>;
	
    AsyncOperation(F&& func) 
	: func_(std::move(func)) {
    }
	
    bool await_ready() {
	result_ = func_();
	return !!result_;
    }
    
    void await_suspend(Strand::Handle coro) {
	coro.promise().state_ = Yield::Resume{};
    }
	
    auto await_resume() const {
	return result_;
    }
	
    const ReturnType& result() const {
	return result_;
    }
	
private:
    F func_;
    ReturnType result_;
};

}; // coros
