// Copyright 2021, 2022, 2023, 2024 by Mark Melton
//

#include <bit>
#include <deque>
#include <gtest/gtest.h>
#include <limits>

#include "core/mp/foreach.h"
#include "coro/stream/stream.h"

static const size_t NumberSamples = 64;

using namespace coro;
using namespace std::chrono_literals;

using ChronoDurations = std::tuple<std::chrono::nanoseconds,
                                   std::chrono::microseconds,
                                   std::chrono::milliseconds,
                                   std::chrono::seconds,
                                   std::chrono::minutes,
                                   std::chrono::hours,
                                   std::chrono::days,
                                   std::chrono::weeks,
                                   std::chrono::months,
                                   std::chrono::years>;

using ChronoTypes = std::tuple<std::chrono::day, std::chrono::month, std::chrono::year>;

using IntegralTypes = std::tuple<int32_t, int64_t, uint16_t, int64_t, __uint128_t>;

using FloatingTypes = std::tuple<float, double>;

TEST(CoroStream, Char) {
    for (auto elem : sampler<char>(-10, 10) | take(NumberSamples)) {
        EXPECT_GE(elem, -10);
        EXPECT_LE(elem, 10);
    }

    for (auto elem : chr::lower() | take(NumberSamples)) {
        EXPECT_TRUE(std::isalpha(elem));
        EXPECT_TRUE(std::islower(elem));
    }

    for (auto elem : chr::upper() | take(NumberSamples)) {
        EXPECT_TRUE(std::isalpha(elem));
        EXPECT_TRUE(std::isupper(elem));
    }

    for (auto elem : chr::alpha() | take(NumberSamples))
        EXPECT_TRUE(std::isalpha(elem));

    for (auto elem : chr::alphanum() | take(NumberSamples))
        EXPECT_TRUE(std::isalnum(elem));

    for (auto elem : chr::binary() | take(NumberSamples))
        EXPECT_TRUE(elem >= '0' and elem <= '1');

    for (auto elem : chr::octal() | take(NumberSamples))
        EXPECT_TRUE(elem >= '0' and elem <= '8');

    for (auto elem : chr::decimal() | take(NumberSamples)) {
        auto dec = elem >= '0' and elem <= '9';
        EXPECT_TRUE(dec);
    }

    for (auto elem : chr::hex() | take(NumberSamples)) {
        auto dec = elem >= '0' and elem <= '9';
        auto alpha = elem >= 'a' and elem <= 'f';
        EXPECT_TRUE(dec or alpha);
    }

    for (auto elem : chr::hex(true) | take(NumberSamples)) {
        auto dec = elem >= '0' and elem <= '9';
        auto alpha = elem >= 'A' and elem <= 'F';
        EXPECT_TRUE(dec or alpha);
    }
}

TEST(CoroStream, Chrono) {
    core::mp::foreach<ChronoDurations>([]<class T>() {
        size_t count{};
        for (auto nanos : sampler<T>(T{-100}, T{+100}) | take(NumberSamples)) {
            ++count;
            EXPECT_GE(nanos, T{-100});
            EXPECT_LE(nanos, T{+100});
        }
        EXPECT_EQ(count, NumberSamples);
    });

    core::mp::foreach<ChronoTypes>([]<class T>() {
        size_t count{};
        for (auto unit : sampler<T>() | take(NumberSamples)) {
            ++count;
            EXPECT_GE(unit, std::numeric_limits<T>::min());
            EXPECT_LE(unit, std::numeric_limits<T>::max());
        }
        EXPECT_EQ(count, NumberSamples);
    });
}

TEST(CoroStream, Bool) {
    size_t count{};
    for (auto elem : sampler<bool>() | take(NumberSamples)) {
        if (elem)
            ++count;
    }
    EXPECT_GE(count, NumberSamples / 4);
    EXPECT_LE(count, 3 * NumberSamples / 4);
}

TEST(CoroStream, Integral) {
    core::mp::foreach<IntegralTypes>([]<class T>() {
        auto rangeG = sampler<T>();
        auto min = rangeG.sample();
        auto max = rangeG.sample();
        if (min > max)
            std::swap(min, max);

        for (auto elem : sampler<T>(min, max) | take(NumberSamples)) {
            EXPECT_GE(elem, min);
            EXPECT_LE(elem, max);
        }
    });
}

TEST(CoroStream, LogNormalIntegral) {
    core::mp::foreach<IntegralTypes>([]<class T>() {
        using U = std::make_unsigned_t<T>;
        size_t max_bits = sizeof(U) * CHAR_BIT;
        size_t big_count{0}, small_count{0};
        for (auto elem : log_sampler<T>() | take(NumberSamples)) {
            U magnitude = elem > 0 ? elem : -elem;
            auto n = max_bits - std::countl_zero(magnitude);

            if (n > max_bits / 2)
                ++big_count;
            else
                ++small_count;
        }
        EXPECT_GE(big_count, NumberSamples / 4);
        EXPECT_GE(small_count, NumberSamples / 4);

        auto rangeG = sampler<size_t>(0, sizeof(T) * CHAR_BIT);
        auto min = rangeG.sample();
        auto max = rangeG.sample();
        if (min > max)
            std::swap(min, max);
        for (auto elem : log_sampler<T>(min, max) | take(NumberSamples)) {
            U magnitude = elem > 0 ? elem : -elem;
            auto n = max_bits - std::countl_zero(magnitude);
            EXPECT_LE(n, max);
        }
    });
}

TEST(CoroStream, LogNormalMagnitudeIntegral) {
    core::mp::foreach<IntegralTypes>([]<class T>() {
        using U = std::make_unsigned_t<T>;
        size_t max_bits = sizeof(U) * CHAR_BIT;
        size_t big_count{0}, small_count{0};
        for (auto elem : log_magnitude_sampler<T>() | take(NumberSamples)) {
            auto n = max_bits - std::countl_zero(elem);
            if (n > max_bits / 2)
                ++big_count;
            else
                ++small_count;
        }
        EXPECT_GE(big_count, NumberSamples / 4);
        EXPECT_GE(small_count, NumberSamples / 4);

        auto rangeG = sampler<size_t>(0, sizeof(T) * CHAR_BIT);
        auto min = rangeG.sample();
        auto max = rangeG.sample();
        if (min > max)
            std::swap(min, max);
        for (auto elem : log_magnitude_sampler<T>(min, max) | take(NumberSamples)) {
            auto n = max_bits - std::countl_zero(elem);
            EXPECT_LE(n, max);
        }
    });
}

TEST(CoroStream, Floating) {
    core::mp::foreach<FloatingTypes>([]<class T>() {
        for (auto elem : sampler<T>(-1.0, +1.0) | take(NumberSamples)) {
            EXPECT_GE(elem, -1.0);
            EXPECT_LE(elem, +1.0);
        }
    });
}

TEST(CoroStream, String) {
    for (auto str : sampler<std::string>() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str)
            EXPECT_TRUE(std::isalpha(c));
    }

    for (auto str : str::lower() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str) {
            EXPECT_TRUE(std::isalpha(c));
            EXPECT_TRUE(std::islower(c));
        }
    }

    for (auto str : str::upper() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str) {
            EXPECT_TRUE(std::isalpha(c));
            EXPECT_TRUE(std::isupper(c));
        }
    }

    for (auto str : str::alpha() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str)
            EXPECT_TRUE(std::isalpha(c));
    }

    for (auto str : str::alphanum() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str)
            EXPECT_TRUE(std::isalnum(c));
    }

    for (auto str : str::binary() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str)
            EXPECT_TRUE(c >= '0' and c <= '1');
    }

    for (auto str : str::octal() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str)
            EXPECT_TRUE(c >= '0' and c <= '9');
    }

    for (auto str : str::decimal() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str) {
            auto dec = c >= '0' and c <= '9';
            EXPECT_TRUE(dec);
        }
    }

    for (auto str : str::hex() | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str) {
            auto dec = c >= '0' and c <= '9';
            auto alpha = c >= 'a' and c <= 'f';
            EXPECT_TRUE(dec or alpha);
        }
    }

    for (auto str : str::hex(true) | take(16)) {
        EXPECT_LE(str.size(), 20);
        for (auto c : str) {
            auto dec = c >= '0' and c <= '9';
            auto alpha = c >= 'A' and c <= 'F';
            EXPECT_TRUE(dec or alpha);
        }
    }
}

TEST(CoroStream, Pair) {
    core::mp::foreach<std::tuple<int, double>>([]<class T>() {
        core::mp::foreach<std::tuple<int, double>>([]<class U>() {
            auto g = Sampler<std::pair<T, U>>{}({0, 0}, {10, 10}) | take(NumberSamples);
            for (auto elem : g) {
                EXPECT_GE(elem.first, 0);
                EXPECT_LE(elem.first, 10);
                EXPECT_GE(elem.second, 0);
                EXPECT_LE(elem.second, 10);
            }
        });
    });
}

TEST(CoroStream, Container) {
    using Types =
        std::tuple<std::vector<int>, std::list<int>, std::deque<int>, std::set<int>>;
    core::mp::foreach<Types>([]<class T>() {
        auto g = sampler<T>(0, 10, -20, 20) | take(NumberSamples);
        for (auto container : g) {
            EXPECT_LE(container.size(), 10);
            for (const auto &elem : container) {
                EXPECT_GE(elem, -20);
                EXPECT_LE(elem, +20);
            }
        }
    });
}

TEST(CoroStream, ContainerMap) {
    auto g = Sampler<std::map<int, double>>{}(0, 20, {0, -1.0}, {100, +1.0});
    for (auto map : std::move(g) | take(NumberSamples)) {
        EXPECT_LE(map.size(), 20);
        for (const auto &[key, value] : map) {
            EXPECT_GE(key, 0);
            EXPECT_LE(key, 100);
            EXPECT_GE(value, -1.0);
            EXPECT_LE(value, +1.0);
        }
    }
}

TEST(CoroStream, ContainerContainer) {
    using Types = std::tuple<std::vector<std::vector<int>>,
                             std::vector<std::list<int>>,
                             std::vector<std::deque<int>>,
                             std::list<std::vector<int>>,
                             std::list<std::list<int>>,
                             std::list<std::deque<int>>,
                             std::deque<std::vector<int>>,
                             std::deque<std::list<int>>,
                             std::deque<std::deque<int>>>;
    core::mp::foreach<Types>([]<class T>() {
        auto g =
            Sampler<size_t>{}(0, 10) * Sampler<typename T::value_type>{}(0, 10, -20, 20) |
            samplerG<T>() | take(NumberSamples);
        for (auto outer : g) {
            EXPECT_LE(outer.size(), 10);
            for (const auto &inner : outer) {
                EXPECT_LE(inner.size(), 10);
                for (const auto &elem : inner) {
                    EXPECT_GE(elem, -20);
                    EXPECT_LE(elem, +20);
                }
            }
        }
    });
}

TEST(CoroStream, VectorPair) {
    using Pair = std::pair<int, double>;
    auto g = sampler<size_t>(0, 20) * sampler_pair(Pair{-10, -1.0}, Pair{+10, +1.0}) |
             sampler_vector() | take(NumberSamples);
    size_t count{0};
    for (auto vec : take(std::move(g), NumberSamples)) {
        ++count;
        EXPECT_LE(vec.size(), 20);
        for (const auto &[a, b] : vec) {
            EXPECT_GE(a, -10);
            EXPECT_LE(a, +10);
            EXPECT_GE(b, -1.0);
            EXPECT_LE(b, +1.0);
        }
    }
    EXPECT_EQ(count, NumberSamples);
}

TEST(CoroStream, PairVector) {
    auto g = sampler<std::vector<int>>(0, 10, -100, +100) *
                 sampler<std::vector<double>>(0, 5, -1.0, +1.0) |
             zip() | take(NumberSamples);
    for (const auto &[v0, v1] : g) {
        EXPECT_LE(v0.size(), 10);
        EXPECT_LE(v1.size(), 5);
        for (const auto &elem : v0) {
            EXPECT_GE(elem, -100);
            EXPECT_LE(elem, +100);
        }
        for (const auto &elem : v1) {
            EXPECT_GE(elem, -1.0);
            EXPECT_LE(elem, +1.0);
        }
    }
}

TEST(CoroStream, Array) {
    auto g = sampler<std::array<int, 4>>() | take(NumberSamples);
    for (const auto &arr : g) {
        EXPECT_EQ(arr.size(), 4);
        for (const auto &elem : arr)
            EXPECT_NE(elem, 0);
        for (auto i = 0; i < arr.size(); ++i) {
            EXPECT_NE(arr[i], 0);
            for (auto j = i + 1; j < arr.size(); ++j)
                EXPECT_NE(arr[i], arr[j]);
        }
    }
}

TEST(CoroStream, Tuple) {
    for (auto [i, s] :
         sampler<std::tuple<unsigned int, std::string>>() | take(NumberSamples)) {
        EXPECT_GE(i, 0);
        EXPECT_GE(s.size(), 0);
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
