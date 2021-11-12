// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include <deque>
#include "coro/stream/stream.h"
#include "core/mp/foreach.h"


static const size_t NumberSamples = 64;

using namespace coro;

using IntegralTypes = std::tuple<int32,int64,uint16,uint64>;
using FloatingTypes = std::tuple<float,real>;

TEST(CoroStream, Adapt)
{
    auto expected = sampler<int>(0, 100) | take(10) | collect<std::vector>();
    auto actual = adapt(expected) | collect<std::vector>();
    EXPECT_EQ(expected, actual);
}

TEST(CoroStream, Alternate)
{
    auto g = iota<int>(11) + iota(10, 10) + iota(10, 20) | alternate();
    size_t count{0};
    for (auto elem : g) {
	auto expected = 10 * (count % 3) + count / 3;
	EXPECT_EQ(elem, expected);
	++count;
    }
    EXPECT_EQ(count, 31);
}

TEST(CoroStream, Char)
{
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

TEST(CoroStream, Choose)
{
    auto g = iota<int>(10) + iota(10, 10) + iota(8, 20) | choose();
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
}

TEST(CoroStream, Constant)
{
    core::mp::foreach<IntegralTypes>([]<class T>() {
	    auto value = sampler<T>().sample();
	    for (auto elem : constant(value) | take(NumberSamples))
		EXPECT_EQ(elem, value);
	});
}

TEST(CoroStream, Integral)
{
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

TEST(CoroStream, Filter)
{
    auto g = sampler<int>(0, 100) | filter([](int n) { return n % 2 == 0; }) | take(NumberSamples);
    for (auto elem : g)
	EXPECT_TRUE(elem % 2 == 0);
}

TEST(CoroStream, Floating)
{
    core::mp::foreach<FloatingTypes>([]<class T>() {
	    for (auto elem : sampler<T>(-1.0, +1.0) | take(NumberSamples)) {
		EXPECT_GE(elem, -1.0);
		EXPECT_LE(elem, +1.0);
	    }
	});
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

TEST(CoroStream, String)
{
    for (auto str : sampler<string>() | take(16)) {
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

    for (auto str : str::hex() | take(16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) {
	    auto dec = c >= '0' and c <= '9';
	    auto alpha = c>= 'a' and c <= 'f';
	    EXPECT_TRUE(dec or alpha);
	}
    }
    
    for (auto str : str::hex(true) | take(16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) {
	    auto dec = c >= '0' and c <= '9';
	    auto alpha = c>= 'A' and c <= 'F';
	    EXPECT_TRUE(dec or alpha);
	}
    }
}

TEST(CoroStream, Sequence)
{
    auto g = (sampler<int>(0, 9) | take(10))
	+ (sampler<int>(10, 19) | take(10))
	+ (sampler<int>(20, 29) | take(10))
	+ (sampler<int>(30, 39) | take(10))
	| sequence();
    int count{0};
    for (auto elem : g) {
	++count;
	auto delta = std::abs(elem - count);
	EXPECT_LE(delta, 10);
    }
    EXPECT_EQ(count, 40);
}

TEST(CoroStream, Pair)
{
    core::mp::foreach<std::tuple<int,real>>([]<class T>() {
	    core::mp::foreach<std::tuple<int,real>>([]<class U>() {
		    auto g = Sampler<std::pair<T,U>>{}({0,0}, {10,10}) | take(NumberSamples);
		    for (auto elem : g) {
			EXPECT_GE(elem.first, 0);
			EXPECT_LE(elem.first, 10);
			EXPECT_GE(elem.second, 0);
			EXPECT_LE(elem.second, 10);
		    }
		});
	});
}

TEST(CoroStream, Container)
{
    using Types = std::tuple<std::vector<int>
			     ,std::list<int>
			     ,std::deque<int>
			     ,std::set<int>
			     >;
    core::mp::foreach<Types>([]<class T>() {
	    auto g = sampler<T>(0, 10, -20, 20) | take(NumberSamples);
	    for (auto container : g) {
		EXPECT_LE(container.size(), 10);
		for (const auto& elem : container) {
		    EXPECT_GE(elem, -20);
		    EXPECT_LE(elem, +20);
		}
	    }
	});
}

TEST(CoroStream, ContainerMap)
{
    auto g = Sampler<std::map<int,real>>{}(0, 20, {0,-1.0}, {100,+1.0});
    for (auto map : std::move(g) | take(NumberSamples)) {
	EXPECT_LE(map.size(), 20);
	for (const auto& [key, value] : map) {
	    EXPECT_GE(key, 0);
	    EXPECT_LE(key, 100);
	    EXPECT_GE(value, -1.0);
	    EXPECT_LE(value, +1.0);
	}
    }
}

TEST(CoroStream, ContainerContainer)
{
    using Types = std::tuple<std::vector<std::vector<int>>
			     ,std::vector<std::list<int>>
			     ,std::vector<std::deque<int>>
			     ,std::list<std::vector<int>>
			     ,std::list<std::list<int>>
			     ,std::list<std::deque<int>>
			     ,std::deque<std::vector<int>>
			     ,std::deque<std::list<int>>
			     ,std::deque<std::deque<int>>
			     >;
    core::mp::foreach<Types>([]<class T>() {
	    auto g = Sampler<size_t>{}(0, 10) * Sampler<typename T::value_type>{}(0, 10, -20, 20)
	        | samplerG<T>()
	        | take(NumberSamples);
	    for (auto outer : g) {
		EXPECT_LE(outer.size(), 10);
		for (const auto& inner : outer) {
		    EXPECT_LE(inner.size(), 10);
		    for (const auto& elem : inner) {
			EXPECT_GE(elem, -20);
			EXPECT_LE(elem, +20);
		    }
		}
	    }
	});
}

TEST(CoroStream, VectorPair)
{
    using Pair = std::pair<int,real>;
    auto g = sampler<size_t>(0, 20) * sampler_pair(Pair{-10,-1.0}, Pair{+10,+1.0})
	| sampler_vector()
	| take(NumberSamples);
    size_t count{0};
    for (auto vec : take(std::move(g), NumberSamples)) {
	++count;
	EXPECT_LE(vec.size(), 20);
	for (const auto& [a, b] : vec) {
	    EXPECT_GE(a, -10);
	    EXPECT_LE(a, +10);
	    EXPECT_GE(b, -1.0);
	    EXPECT_LE(b, +1.0);
	}
    }
    EXPECT_EQ(count, NumberSamples);
}

TEST(CoroStream, PairVector)
{
    auto g = sampler<vector<int>>(0, 10, -100, +100) * sampler<vector<real>>(0, 5, -1.0, +1.0)
	| zip()
	| take(NumberSamples);
    for (const auto& [v0, v1] : g) {
	EXPECT_LE(v0.size(), 10);
	EXPECT_LE(v1.size(), 5);
	for (const auto& elem : v0) {
	    EXPECT_GE(elem, -100);
	    EXPECT_LE(elem, +100);
	}
	for (const auto& elem : v1) {
	    EXPECT_GE(elem, -1.0);
	    EXPECT_LE(elem, +1.0);
	}
    }
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



