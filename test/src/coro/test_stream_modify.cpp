// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/stream/stream.h"

using namespace coro;
static const size_t NumberSamples = 5;

class Environment : public ::testing::Environment {
public:
    using Sample = std::vector<int>;
    using SamplePair = std::pair<Sample,Sample>;
    
    Environment()
	: generator_(sampler<int>(-10, 10) | group(10)) {
    }
    ~Environment() override { }
    void SetUp() override { }
    void TearDown() override { }
    Sample sample() { return generator_.sample(); }
    SamplePair sample_pair() {
	auto s0 = sample();
	auto s1 = s0 | transform([](int n) { return 2 * n; }) | collect<std::vector>();
	return std::make_pair(s0, s1);
    }
    Generator<std::vector<int>> generator_;
};

Environment *env{nullptr};

TEST(CoroStream, Adapt)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	EXPECT_NE(data, expected);
    
	adapt(data) | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
    }
}

// TEST(CoroStream, Alternate)
// {
//     auto g = iota<int>(11) * iota(10, 10) * iota(10, 20) | alternate();
//     size_t count{0};
//     for (auto elem : g) {
// 	auto expected = 10 * (count % 3) + count / 3;
// 	EXPECT_EQ(elem, expected);
// 	++count;
//     }
//     EXPECT_EQ(count, 31);
// }

TEST(CoroStream, Apply)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	EXPECT_NE(data, expected);
    
	data | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
    }
}

// TEST(CoroStream, Choose)
// {
//     auto g = iota<int>(10) * iota(10, 10) * iota(8, 20) | choose();
//     std::set<int> unique;
//     size_t count{0};
//     for (auto elem : g) {
// 	EXPECT_GE(elem, 0);
// 	EXPECT_LT(elem, 28);
// 	EXPECT_FALSE(unique.contains(elem));
// 	unique.insert(elem);
// 	++count;
//     }
//     EXPECT_EQ(count, 28);
// }

// TEST(CoroStream, Collect)
// {
//     auto vec = sampler<int>(0, 100) | take(4) | collect<std::vector>();
//     EXPECT_EQ(vec.size(), 4);
//     for (auto elem : vec) {
// 	EXPECT_GE(elem, 0);
// 	EXPECT_LE(elem, 100);
//     }

//     auto vec_copy = vec | collect<std::vector>();
//     EXPECT_EQ(vec, vec_copy);
// }

// TEST(CoroStream, Filter)
// {
//     auto g = sampler<int>(0, 100) | filter([](int n) { return n % 2 == 0; }) | take(NumberSamples);
//     for (auto elem : g)
// 	EXPECT_TRUE(elem % 2 == 0);
// }

// TEST(CoroStream, Group)
// {
//     for (auto vec : sampler<int>(0, 100) | group(4) | take(NumberSamples)) {
// 	EXPECT_EQ(vec.size(), 4);
// 	for (auto elem : vec) {
// 	    EXPECT_GE(elem, 0);
// 	    EXPECT_LE(elem, 100);
// 	}
//     }

//     auto g = sampler<real>(-1, +1) * sampler<size_t>(1, 8) | group() | take(NumberSamples);
//     for (const auto& vec : g) {
// 	EXPECT_GT(vec.size(), 0);
// 	EXPECT_LE(vec.size(), 8);
// 	for (auto x : vec) {
// 	    EXPECT_GE(x, -1);
// 	    EXPECT_LE(x, +1);
// 	}
//     }
// }

// TEST(CoroStream, Iota)
// {
//     auto c = iota(3, 10, 2) | collect<std::vector>();
//     ASSERT_EQ(c.size(), 3);
//     EXPECT_EQ(c[0], 10);
//     EXPECT_EQ(c[1], 12);
//     EXPECT_EQ(c[2], 14);
// }

// TEST(CoroStream, Repeat)
// {
//     for (auto value : sampler<int>(0, 100) | take(NumberSamples)) {
// 	auto g = repeat(value) | take(1000);
// 	size_t count{0};
// 	for (auto elem : g) {
// 	    EXPECT_EQ(elem, value);
// 	    ++count;
// 	}
// 	EXPECT_EQ(count, 1000);
//     }
// }

// TEST(CoroStream, RepeatCount)
// {
//     for (auto [n, m] : sampler<int>(0, 100) * sampler<int>(0, 10) | zip() | take(NumberSamples)) {
// 	auto g = repeat(n, m);
// 	size_t count{0};
// 	for (auto elem : g) {
// 	    EXPECT_EQ(elem, n);
// 	    ++count;
// 	}
// 	EXPECT_EQ(count, m);
//     }
// }

// TEST(CoroStream, Sequence)
// {
//     auto g = (sampler<int>(0, 9) | take(10))
// 	* (sampler<int>(10, 19) | take(10))
// 	* (sampler<int>(20, 29) | take(10))
// 	* (sampler<int>(30, 39) | take(10))
// 	| sequence();
//     int count{0};
//     for (auto elem : g) {
// 	++count;
// 	auto delta = std::abs(elem - count);
// 	EXPECT_LE(delta, 10);
//     }
//     EXPECT_EQ(count, 40);
// }

// TEST(CoroStream, Take)
// {
//     auto g = iota<int>(99) | take(5);
//     size_t count{0};
//     for (auto&& elem : g) {
// 	EXPECT_EQ(elem, count);
// 	++count;
//     }
//     EXPECT_EQ(count, 5);
// }

// TEST(CoroStream, Transform)
// {
//     auto g = iota<int>(5) | transform([](int n) { return n * n; });
//     size_t count{0};
//     for (auto&& elem : g) {
// 	EXPECT_EQ(elem, count * count);
// 	++count;
//     }
// }

// TEST(CoroStream, Unique)
// {
//     auto g = sampler<int>(0, 100) | unique([](int n) { return n % 11; }) | take(11);
//     std::set<int> s;
//     for (auto elem : g) {
// 	EXPECT_FALSE(s.contains(elem % 11));
// 	s.insert(elem % 11);
//     }
//     EXPECT_EQ(s.size(), 11);
// }

// TEST(CoroStream, Zip)
// {
//     auto g = sampler<int>(-20, +20)
// 	* sampler<real>(-1, +1)
// 	* sampler<char>('a','z')
// 	| zip()
// 	| take(NumberSamples);
//     size_t count{0};
//     for (const auto& [a, b, c] : g) {
// 	++count;
// 	EXPECT_GE(a, -20);
// 	EXPECT_LE(a, +20);
// 	EXPECT_GE(b, -1.0);
// 	EXPECT_LE(b, +1.0);
// 	EXPECT_GE(c, 'a');
// 	EXPECT_LE(c, 'z');
//     }
//     EXPECT_EQ(count, NumberSamples);
// }

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}



