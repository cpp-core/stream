// Copyright (C) 2021 by Mark Melton
//

#include "coro/stream/uniform_chrono.h"
#include "core/chrono.h"
#include "core/algo/random.h"

namespace costr {

using namespace chron;

coro::Generator<Date> Uniform<Date>::operator()(Date start, Date end) const {
    auto sday = sys_days(start).time_since_epoch().count();
    auto eday = sys_days(end).time_since_epoch().count();
    auto dist = std::uniform_int_distribution<int>(0, eday - sday);
    while (true) {
	auto n = dist(core::rng());
	auto date = Date{sys_days(start) + InDays{n}};
	co_yield date;
    }
    co_return;
}

// template<class T>
// requires core::mp::is_same_template_v<T, std::chrono::time_point>
// coro::Generator<T> Uniform<T>::operator()(T start, T end) const {
//     auto sn = start.time_since_epoch().count();
//     auto en = end.time_since_epoch().count();
//     auto dist = std::uniform_int_distribution<int>(0, en - sn);
//     while (true) {
// 	auto n = sn + dist(core::rng());
// 	auto tp = T{n};
// 	co_yield tp;
//     }
//     co_return;
// }

};
