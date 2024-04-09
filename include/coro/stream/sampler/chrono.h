// Copyright 2021, 2022, 2023, 2024 by Mark Melton
//

#pragma once
#include <chrono>
#include <limits>
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

template <class T>
    requires(std::is_same_v<T, std::chrono::day> or
             std::is_same_v<T, std::chrono::month> or
             std::is_same_v<T, std::chrono::year>)
struct Sampler<T> {
    using TickType =
        std::conditional_t<std::is_same_v<T, std::chrono::year>, int, unsigned int>;
    Generator<T> operator()(T min = std::numeric_limits<T>::min(),
                            T max = std::numeric_limits<T>::max()) const {
        std::uniform_int_distribution<TickType> dist((TickType)min, (TickType)max);
        while (true)
            co_yield T{dist(coro::detail::rng())};
        co_return;
    }
};

}; // namespace coro
