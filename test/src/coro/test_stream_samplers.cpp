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

TEST(CoroStream, Floating)
{
    core::mp::foreach<FloatingTypes>([]<class T>() {
	    for (auto elem : sampler<T>(-1.0, +1.0) | take(NumberSamples)) {
		EXPECT_GE(elem, -1.0);
		EXPECT_LE(elem, +1.0);
	    }
	});
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



