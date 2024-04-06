// Copyright 2021, 2022, 2023, 2024 by Mark Melton
//

#pragma once
#include <chrono>
#include <type_traits>

#include "core/mp/same.h"
#include "coro/stream/detail/random.h"
#include "coro/stream/sampler.h"

namespace coro {

template <class T>
    requires core::mp::is_same_template_v<T, std::chrono::duration>
struct Sampler<T> {
    using TickType = typename T::rep;
    Generator<T> operator()(T min = T::min(), T max = T::max()) const {
        std::uniform_int_distribution<TickType> dist(min.count(), max.count());
        while (true)
            co_yield T{dist(coro::detail::rng())};
        co_return;
    }
};

}; // namespace coro
