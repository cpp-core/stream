// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/consumer.h"
#include "coro/stream/stream.h"

using namespace coro;
static const size_t NumberSamples = 64;

coro::Consumer<int,int> add_one() {
    while (true) {
	auto x = co_yield coro::input{};
	co_yield x + 1;
    }
    co_return;
}

coro::Consumer<char> append(string& s) {
    while (true) {
	auto c = co_yield coro::input{};
	s.push_back(c);
	co_yield coro::input_done{};
    }
}

TEST(CoroConsumer, Base)
{
    auto p = add_one();
    for (auto input : take(sampler<int>(), NumberSamples)) {
	p.push(input);
	EXPECT_EQ(p(), input + 1);
    }

    string result;
    auto s = append(result);
    s.push('a');
    s.push('b');
    s.push('c');
    EXPECT_EQ(result, "abc"s);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



