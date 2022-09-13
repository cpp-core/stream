// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/generator.h"
#include "coro/stream/stream.h"

static const size_t NumberSamples = 2;

coro::Generator<int> iota(size_t n) {
    for (auto i = 0; i < n; ++i)
	co_yield i;
}

TEST(CoroGenerator, NextCall)
{
    for (auto n : coro::sampler<int>(0, 128) | coro::take(NumberSamples)) {
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

coro::Generator<int> recursive_counter(int n) {
    if (n > 0)
	co_yield recursive_counter(n - 1);
    co_yield n;
}

TEST(CoroGenerator, RecursiveCounter)
{
    size_t count{0}, max_count{1'000'000};
    for (auto i : recursive_counter(max_count)) 
	EXPECT_EQ(count++, i);
    EXPECT_EQ(count - 1, max_count);
}

coro::Generator<int> fibonacci(int count, int a = 0, int b = 1) {
    co_yield a;
    if (--count > 0)
	co_yield fibonacci(count, b, a + b);
}

TEST(CoroGenerator, RecursiveFibonacci)
{
    size_t idx{0};
    std::vector<int> fibs = { 0, 1, 1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144 };
    for (auto i : fibonacci(fibs.size())) {
	EXPECT_EQ(i, fibs[idx]);
	++idx;
    }
}

TEST(CoroGenerator, Ranges)
{
    // auto g = counter(20) | v::take(5);
    // for (auto i : g)
    // 	cout << i << endl;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



