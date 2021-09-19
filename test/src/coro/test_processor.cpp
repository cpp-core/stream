// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/processor.h"
#include "coro/stream/stream.h"

using namespace costr;
static const size_t NumberSamples = 64;

coro::Processor<int,int> add_one() {
    while (true) {
	auto x = co_yield coro::input_token{};
	co_yield x + 1;
    }
    co_return;
}

coro::Processor<int,char> append(string& s) {
    while (true) {
	auto c = co_yield coro::input_token{};
	s.push_back(c);
	co_yield coro::ready_token{};
    }
}

TEST(CoroProcessor, Base)
{
    auto p = add_one();
    for (auto input : take(uniform<int>(), NumberSamples)) {
	p.sink(input);
	EXPECT_EQ(p(), input + 1);
    }

    string result;
    auto s = append(result);
    s.sink('a');
    s.sink('b');
    s.sink('c');
    EXPECT_EQ(result, "abc"s);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



