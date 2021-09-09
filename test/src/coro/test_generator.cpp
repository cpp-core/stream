// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/generator.h"
#include "core/range/sample.h"

static const size_t NumberSamples = 2;

co::Generator<int> iota(size_t n) {
    for (auto i = 0; i < n; ++i)
	co_yield i;
}

TEST(CoroGenerator, NextCall)
{
    for (auto n : cr::uniform(0, 128) | v::take(NumberSamples)) {
	auto loop = iota(n);
	size_t count{0};
	while (loop.next())
	    EXPECT_EQ(count++, loop());
	EXPECT_EQ(count, n);
    }
}

TEST(CoroGenerator, RangeBasedForLoop)
{
    size_t count{0};
    for (auto n : iota(5))
	EXPECT_EQ(count++, n);
    EXPECT_EQ(count, 5);
}

// TEST(CoroGenerator, Range)
// {
//     auto g = iota(10) | v::take(5);
// }

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



