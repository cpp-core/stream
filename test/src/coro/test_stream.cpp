// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include <deque>
#include "coro/stream/stream.h"
#include "core/mp/foreach.h"
#include "core/type/fixed.h"

using namespace coro;
static const size_t NumberSamples = 64;

TEST(CoroStream, Adapt)
{
    auto expected = sampler<int>(0, 100) | take(10) | collect<std::vector>();
    auto actual = adapt(expected) | collect<std::vector>();
    EXPECT_EQ(expected, actual);
}

TEST(CoroStream, AdaptFixedVector)
{
    core::Fixed<vector<int>> expected{1, 2, 3, 4};
    auto actual = adapt(expected) | collect<std::vector>();
    EXPECT_EQ(expected, actual);
}

TEST(CoroStream, Alternate)
{
    auto g = iota<int>(11) * iota(10, 10) * iota(10, 20) | alternate();
    size_t count{0};
    for (auto elem : g) {
	auto expected = 10 * (count % 3) + count / 3;
	EXPECT_EQ(elem, expected);
	++count;
    }
    EXPECT_EQ(count, 31);
}

TEST(CoroStream, AlternateFixedVector)
{
    std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
	b{10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
	c{20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
    auto g = a * b * c | alternate();
    size_t count{0};
    for (auto elem : g) {
	auto expected = 10 * (count % 3) + count / 3;
	EXPECT_EQ(elem, expected);
	++count;
    }
    EXPECT_EQ(count, 31);
}

TEST(CoroStream, Apply)
{
    auto count{0};
    sampler<int>(0, 100) | take(10) | apply([&](int n) { ++count; });
    EXPECT_EQ(count, 10);
}

TEST(CoroStream, ApplyFixedVector)
{
    auto count{0};
    core::Fixed<std::vector<int>> data{1, 2, 3, 4};
    data | apply([&](int n) { ++count; });
    EXPECT_EQ(count, 4);
}

TEST(CoroStream, Choose)
{
    auto g = iota<int>(10) * iota(10, 10) * iota(8, 20) | choose();
    std::set<int> unique;
    size_t count{0};
    for (auto elem : g) {
	EXPECT_GE(elem, 0);
	EXPECT_LT(elem, 28);
	EXPECT_FALSE(unique.contains(elem));
	unique.insert(elem);
	++count;
    }
    EXPECT_EQ(count, 28);
}

TEST(CoroStream, ChooseFixedVector)
{
    std::vector<int> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
	b{10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
	c{20, 21, 22, 23, 24, 25, 26, 27};
    auto g = a * b * c | choose();
    std::set<int> unique;
    size_t count{0};
    for (auto elem : g) {
	EXPECT_GE(elem, 0);
	EXPECT_LT(elem, 28);
	EXPECT_FALSE(unique.contains(elem));
	unique.insert(elem);
	++count;
    }
    EXPECT_EQ(count, 28);
}

TEST(CoroStream, Collect)
{
    auto vec = sampler<int>(0, 100) | take(4) | collect<std::vector>();
    EXPECT_EQ(vec.size(), 4);
    for (auto elem : vec) {
	EXPECT_GE(elem, 0);
	EXPECT_LE(elem, 100);
    }

    auto vec_copy = vec | collect<std::vector>();
    EXPECT_EQ(vec, vec_copy);
}

TEST(CoroStream, CollectFixedVector)
{
    core::Fixed<std::vector<int>> data{1, 2, 3, 4};
    auto vec = data | collect<std::vector>();
    EXPECT_EQ(vec, data);
}

TEST(CoroStream, Filter)
{
    auto g = sampler<int>(0, 100) | filter([](int n) { return n % 2 == 0; }) | take(NumberSamples);
    for (auto elem : g)
	EXPECT_TRUE(elem % 2 == 0);
}

TEST(CoroStream, FilterFixedVector)
{
    core::Fixed<std::vector<int>> data{0, 1, 2, 3};
    auto g = data | filter([](int n) { return n % 2 == 0; }) | take(NumberSamples);
    for (auto elem : g)
	EXPECT_TRUE(elem % 2 == 0);
}

TEST(CoroStream, Group)
{
    for (auto vec : sampler<int>(0, 100) | group(4) | take(NumberSamples)) {
	EXPECT_EQ(vec.size(), 4);
	for (auto elem : vec) {
	    EXPECT_GE(elem, 0);
	    EXPECT_LE(elem, 100);
	}
    }

    auto g = sampler<real>(-1, +1) * sampler<size_t>(1, 8) | group() | take(NumberSamples);
    for (const auto& vec : g) {
	EXPECT_GT(vec.size(), 0);
	EXPECT_LE(vec.size(), 8);
	for (auto x : vec) {
	    EXPECT_GE(x, -1);
	    EXPECT_LE(x, +1);
	}
    }
}

TEST(CoroStream, GroupTuple)
{
    for (auto [a, b] : sampler<int>(0, 100) | group_tuple<2>() | take(NumberSamples)) {
	EXPECT_GE(a, 0);
	EXPECT_LE(a, 100);
	EXPECT_GE(b, 0);
	EXPECT_LE(b, 100);
    }
}

TEST(CoroStream, Iota)
{
    auto c = iota(3, 10, 2) | collect<std::vector>();
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 10);
    EXPECT_EQ(c[1], 12);
    EXPECT_EQ(c[2], 14);
}

TEST(CoroStream, IotaStart)
{
    auto c = iota(10, 10) | collect<std::vector>();
    ASSERT_EQ(c.size(), 10);
    for (auto i = 0; i < 10; ++i)
	EXPECT_EQ(c[i], i + 10);
}

TEST(CoroStream, Range)
{
    auto c = range(10, 14, 2) | collect<std::vector>();
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 10);
    EXPECT_EQ(c[1], 12);
    EXPECT_EQ(c[2], 14);
}


TEST(CoroStream, Repeat)
{
    for (auto value : sampler<int>(0, 100) | take(NumberSamples)) {
	auto g = repeat(value) | take(1000);
	size_t count{0};
	for (auto elem : g) {
	    EXPECT_EQ(elem, value);
	    ++count;
	}
	EXPECT_EQ(count, 1000);
    }
}

TEST(CoroStream, RepeatCount)
{
    for (auto [n, m] : sampler<int>(0, 100) * sampler<int>(0, 10) | zip() | take(NumberSamples)) {
	auto g = repeat(n, m);
	size_t count{0};
	for (auto elem : g) {
	    EXPECT_EQ(elem, n);
	    ++count;
	}
	EXPECT_EQ(count, m);
    }
}

TEST(CoroStream, Sequence)
{
    auto g = (sampler<int>(0, 9) | take(10))
	* (sampler<int>(10, 19) | take(10))
	* (sampler<int>(20, 29) | take(10))
	* (sampler<int>(30, 39) | take(10))
	| sequence();
    int count{0};
    for (auto elem : g) {
	++count;
	auto delta = std::abs(elem - count);
	EXPECT_LE(delta, 10);
    }
    EXPECT_EQ(count, 40);
}

TEST(CoroStream, SequenceFixedVector)
{
    core::Fixed<std::vector<int>> a{0, 1, 2, 3, 4, 5, 6, 7, 8, 9},
	b{10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
	c{20, 21, 22, 23, 24, 25, 26, 27, 28, 29};
    auto g = a * b * c | sequence();
    int count{0};
    for (auto elem : g) {
	EXPECT_EQ(elem, count);
	++count;
    }
    EXPECT_EQ(count, 30);
}

TEST(CoroStream, Take)
{
    auto g = iota<int>(99) | take(5);
    size_t count{0};
    for (auto&& elem : g) {
	EXPECT_EQ(elem, count);
	++count;
    }
    EXPECT_EQ(count, 5);
}

TEST(CoroStream, TakeFixedVector)
{
    core::Fixed<std::vector<int>> data{0, 1, 2, 3, 4, 5, 6, 7};
    auto g = data | take(5);
    size_t count{0};
    for (auto&& elem : g) {
	EXPECT_EQ(elem, count);
	++count;
    }
    EXPECT_EQ(count, 5);
}

TEST(CoroStream, Transform)
{
    auto g = iota<int>(5) | transform([](int n) { return n * n; });
    size_t count{0};
    for (auto&& elem : g) {
	EXPECT_EQ(elem, count * count);
	++count;
    }
}

TEST(CoroStream, TransformFixedVector)
{
    core::Fixed<std::vector<int>> data{0, 1, 2, 3, 4};
    auto g = data | transform([](int n) { return n * n; });
    size_t count{0};
    for (auto&& elem : g) {
	EXPECT_EQ(elem, count * count);
	++count;
    }
}

TEST(CoroStream, Unique)
{
    auto g = sampler<int>(0, 100) | unique([](int n) { return n % 11; }) | take(11);
    std::set<int> s;
    for (auto elem : g) {
	EXPECT_FALSE(s.contains(elem % 11));
	s.insert(elem % 11);
    }
    EXPECT_EQ(s.size(), 11);
}

TEST(CoroStream, UniqueFixedVector)
{
    core::Fixed<std::vector<int>> data{0, 1, 2, 3, 4, 5, 6};
    auto g = data | unique([](int n) { return n % 3; }) | take(3);
    std::set<int> s;
    for (auto elem : g) {
	EXPECT_FALSE(s.contains(elem % 3));
	s.insert(elem % 3);
    }
    EXPECT_EQ(s.size(), 3);
}

TEST(CoroStream, Zip)
{
    auto g = sampler<int>(-20, +20)
	* sampler<real>(-1, +1)
	* sampler<char>('a','z')
	| zip()
	| take(NumberSamples);
    size_t count{0};
    for (const auto& [a, b, c] : g) {
	++count;
	EXPECT_GE(a, -20);
	EXPECT_LE(a, +20);
	EXPECT_GE(b, -1.0);
	EXPECT_LE(b, +1.0);
	EXPECT_GE(c, 'a');
	EXPECT_LE(c, 'z');
    }
    EXPECT_EQ(count, NumberSamples);
}

TEST(CoroStream, ZipFixedVector)
{
    core::Fixed<std::vector<int>> a{-1, 0, +1};
    core::Fixed<std::vector<real>> b{-10.0, 0.0, 10.0};
    core::Fixed<std::vector<char>> c{'a', 'b', 'c', 'd'};
    
    auto g = a * b * c | zip();
    size_t count{0};
    for (const auto& [x, y, z] : g) {
	EXPECT_EQ(x, a[count]);
	EXPECT_EQ(y, b[count]);
	EXPECT_EQ(z, c[count]);
	++count;
    }
    EXPECT_EQ(count, 3);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



