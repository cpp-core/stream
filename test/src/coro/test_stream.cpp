// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include <deque>
#include "coro/stream/stream.h"
#include "core/mp/foreach.h"


static const size_t NumberSamples = 64;

using namespace costr;

using IntegralTypes = std::tuple<int16,int32,int64,uint16,uint32,uint64>;
using FloatingTypes = std::tuple<float,real>;

TEST(Costr, Char)
{
    for (auto elem : take(sampler<char>(-10, 10), NumberSamples)) {
	EXPECT_GE(elem, -10);
	EXPECT_LE(elem, 10);
    }

    for (auto elem : take(chr::lower(), NumberSamples)) {
	EXPECT_TRUE(std::isalpha(elem));
	EXPECT_TRUE(std::islower(elem));
    }
    
    for (auto elem : take(chr::upper(), NumberSamples)) {
	EXPECT_TRUE(std::isalpha(elem));
	EXPECT_TRUE(std::isupper(elem));
    }
    
    for (auto elem : take(chr::alpha(), NumberSamples))
	EXPECT_TRUE(std::isalpha(elem));
    
    for (auto elem : take(chr::alphanum(), NumberSamples))
	EXPECT_TRUE(std::isalnum(elem));
    
    for (auto elem : take(chr::hex(), NumberSamples)) {
	auto dec = elem >= '0' and elem <= '9';
	auto alpha = elem >= 'a' and elem <= 'f';
	EXPECT_TRUE(dec or alpha);
    }

    for (auto elem : take(chr::hex(true), NumberSamples)) {
	auto dec = elem >= '0' and elem <= '9';
	auto alpha = elem >= 'A' and elem <= 'F';
	EXPECT_TRUE(dec or alpha);
    }
}

TEST(Costr, Constant)
{
    core::mp::foreach<IntegralTypes>([]<class T>() {
	    auto value = sampler<T>().sample();
	    auto g = constant(value);
	    for (auto elem : take(std::move(g), NumberSamples))
		EXPECT_EQ(elem, value);
	});
}

TEST(Costr, Integral)
{
    core::mp::foreach<IntegralTypes>([]<class T>() {
	    auto rangeG = sampler<T>();
	    auto min = rangeG.sample();
	    auto max = rangeG.sample();
	    if (min > max)
		std::swap(min, max);

	    for (auto elem : take(sampler<T>(min, max), NumberSamples)) {
		EXPECT_GE(elem, min);
		EXPECT_LE(elem, max);
	    }
	});
}

TEST(Costr, Floating)
{
    core::mp::foreach<FloatingTypes>([]<class T>() {
	    for (auto elem : take(sampler<T>(-1.0, +1.0), NumberSamples)) {
		EXPECT_GE(elem, -1.0);
		EXPECT_LE(elem, +1.0);
	    }
	});
}

TEST(Costr, String)
{
    for (auto str : take(sampler<string>(), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str)
	    EXPECT_TRUE(std::isalpha(c));
    }

    for (auto str : take(str::lower(), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) {
	    EXPECT_TRUE(std::isalpha(c));
	    EXPECT_TRUE(std::islower(c));
	}
    }
    
    for (auto str : take(str::upper(), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) {
	    EXPECT_TRUE(std::isalpha(c));
	    EXPECT_TRUE(std::isupper(c));
	}
    }
    
    for (auto str : take(str::alpha(), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) 
	    EXPECT_TRUE(std::isalpha(c));
    }
    
    for (auto str : take(str::alphanum(), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) 
	    EXPECT_TRUE(std::isalnum(c));
    }

    for (auto str : take(str::hex(), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) {
	    auto dec = c >= '0' and c <= '9';
	    auto alpha = c>= 'a' and c <= 'f';
	    EXPECT_TRUE(dec or alpha);
	}
    }
    
    for (auto str : take(str::hex(true), 16)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) {
	    auto dec = c >= '0' and c <= '9';
	    auto alpha = c>= 'A' and c <= 'F';
	    EXPECT_TRUE(dec or alpha);
	}
    }
}

TEST(Costr, Pair)
{
    core::mp::foreach<std::tuple<int,real>>([]<class T>() {
	    core::mp::foreach<std::tuple<int,real>>([]<class U>() {
		    auto g = Sampler<std::pair<T,U>>{}({0,0}, {10,10});
		    for (auto elem : take(std::move(g), NumberSamples)) {
			EXPECT_GE(elem.first, 0);
			EXPECT_LE(elem.first, 10);
			EXPECT_GE(elem.second, 0);
			EXPECT_LE(elem.second, 10);
		    }
		});
	});
}

TEST(Costr, Zip)
{
    auto g0 = sampler<int>(-20, +20);
    auto g1 = sampler<real>(-1.0, +1.0);
    auto g = zip(std::move(g0), std::move(g1));
    for (const auto& [a, b] : take(std::move(g), NumberSamples)) {
	EXPECT_GE(a, -20);
	EXPECT_LE(a, +20);
	EXPECT_GE(b, -1.0);
	EXPECT_LE(b, +1.0);
    }
}

TEST(Costr, ZipPair)
{
    auto g0 = sampler<int>(-20, +20);
    auto g1 = sampler<real>(-1.0, +1.0);
    auto g = zip_pair(std::move(g0), std::move(g1));
    for (const auto& [a, b] : take(std::move(g), NumberSamples)) {
	EXPECT_GE(a, -20);
	EXPECT_LE(a, +20);
	EXPECT_GE(b, -1.0);
	EXPECT_LE(b, +1.0);
    }
}

TEST(Costr, Container)
{
    using Types = std::tuple<std::vector<int>
			     ,std::list<int>
			     ,std::deque<int>
			     ,std::set<int>
			     >;
    core::mp::foreach<Types>([]<class T>() {
	    auto g = sampler<T>(0, 10, -20, 20);
	    for (auto container : take(std::move(g), NumberSamples)) {
		EXPECT_LE(container.size(), 10);
		for (const auto& elem : container) {
		    EXPECT_GE(elem, -20);
		    EXPECT_LE(elem, +20);
		}
	    }
	});
}

TEST(Costr, ContainerMap)
{
    auto g = Sampler<std::map<int,real>>{}(0, 20, {0,-1.0}, {100,+1.0});
    for (auto map : take(std::move(g), NumberSamples)) {
	EXPECT_LE(map.size(), 20);
	for (const auto& [key, value] : map) {
	    EXPECT_GE(key, 0);
	    EXPECT_LE(key, 100);
	    EXPECT_GE(value, -1.0);
	    EXPECT_LE(value, +1.0);
	}
    }
}

TEST(Costr, ContainerContainer)
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
	    auto g_size = Sampler<size_t>{}(0, 10);
	    auto g_elem = Sampler<typename T::value_type>{}(0, 10, -20, 20);
	    auto src = Sampler<T>{}(std::move(g_size), std::move(g_elem));
	    for (auto outer : take(std::move(src), NumberSamples)) {
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

TEST(Costr, VectorPair)
{
    auto g_size = Sampler<size_t>{}(0, 20);
    auto g_elem = Sampler<std::pair<int,real>>{}({-10,-1.0}, {+10,+1.0});
    auto g = Sampler<std::vector<std::pair<int,real>>>{}(std::move(g_size), std::move(g_elem));
    for (auto vec : take(std::move(g), NumberSamples)) {
	EXPECT_LE(vec.size(), 20);
	for (const auto& [a, b] : vec) {
	    EXPECT_GE(a, -10);
	    EXPECT_LE(a, +10);
	    EXPECT_GE(b, -1.0);
	    EXPECT_LE(b, +1.0);
	}
    }
}

TEST(Costr, PairVector)
{
    auto gv0 = sampler<vector<int>>(0, 10, -100, +100);
    auto gv1 = sampler<vector<real>>(0, 5, -1.0, +1.0);
    auto g = zip_pair(std::move(gv0), std::move(gv1));
    for (const auto& [v0, v1] : take(std::move(g), NumberSamples)) {
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

TEST(Costr, Iota)
{
    auto c = to_container(iota(3, 10, 2));
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 10);
    EXPECT_EQ(c[1], 12);
    EXPECT_EQ(c[2], 14);
}

TEST(Costr, Range)
{
    auto c = to_container(range(10, 14, 2));
    ASSERT_EQ(c.size(), 3);
    EXPECT_EQ(c[0], 10);
    EXPECT_EQ(c[1], 12);
    EXPECT_EQ(c[2], 14);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



