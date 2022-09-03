// Copyright (C) 2017, 2018, 2019, 2021, 2022 by Mark Melton
//

#pragma once
#include <experimental/coroutine>
#include "core/util/common.h"

namespace coro
{

template<class P>
using handle = std::experimental::coroutine_handle<P>;

using suspend_always = std::experimental::suspend_always;
using suspend_never = std::experimental::suspend_never;

struct input {};
struct input_done {};

}; // coro
