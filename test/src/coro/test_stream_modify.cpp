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

TEST(CoroStreamModify, Adapt)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	EXPECT_NE(data, expected);
    
	adapt(data) | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
    }
}

TEST(CoroStreamModify, Alternate)
{
    // auto [data, expected] = env->sample_pair();
    // auto [data2, expected2] = env->sample_pair();
    // adapt(data) * adapt(data2) | alternate() | apply([](int& n) { n *= 2; });
}

TEST(CoroStreamModify, Apply)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	EXPECT_NE(data, expected);
    
	data | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
    }
}

TEST(CoroStreamModify, Choose)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	auto [data2, expected2] = env->sample_pair();
	adapt(data) * adapt(data2) | choose() | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
	EXPECT_EQ(data2, expected2);
    }
}

TEST(CoroStreamModify, Filter)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	data | filter([](int n) { return true; }) | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
    }
}

TEST(CoroStreamModify, Sequence)
{

    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	auto [data2, expected2] = env->sample_pair();
	adapt(data) * adapt(data2) | sequence() | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
	EXPECT_EQ(data2, expected2);
    }
}

TEST(CoroStreamModify, Take)
{
    for (auto i = 0; i < NumberSamples; ++i) {
	auto [data, expected] = env->sample_pair();
	data | take(data.size()) | apply([](int& n) { n *= 2; });
	EXPECT_EQ(data, expected);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    env = new Environment;
    AddGlobalTestEnvironment(env);
    return RUN_ALL_TESTS();
}



