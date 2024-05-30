// Copyright 2024 by Mark Melton
//

#pragma once
#include <random>

#include "coro/stream/detail/random.h"
#include "coro/stream/stream.h"

namespace coro {

template <Stream S, class U = std::optional<stream_value_t<S>>>
Generator<U &&> optionalize(S source, double probability) {
    std::uniform_real_distribution<double> dist(0.0, 1.0);
    for (auto &&elem : source) {
        if (dist(detail::rng()) > probability)
            co_yield elem;
        else
            co_yield U{};
    }
    co_return;
}

inline auto optionalize(double probability) {
    return [=]<Stream S>(S &&source) {
        return optionalize(std::forward<S>(source), probability);
    };
};

}; // coro
