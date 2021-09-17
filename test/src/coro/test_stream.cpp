// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/stream/stream.h"
#include "core/json/nljson.h"
#include "core/mp/foreach.h"
#include "core/mp/same.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

static const size_t NumberSamples = 64;

using namespace costr;

using IntegralTypes = std::tuple<int16,int32,int64,uint16,uint32,uint64>;
using FloatingTypes = std::tuple<float,real>;

TEST(Costr, Constant)
{
    core::mp::foreach<IntegralTypes>([]<class T>() {
	    auto value = *uniform<T>().begin();
	    auto g = constant(value);
	    for (auto elem : take(std::move(g), NumberSamples))
		EXPECT_EQ(elem, value);
	});
}

TEST(Costr, Integral)
{
    core::mp::foreach<IntegralTypes>([]<class T>() {
	    auto iter = uniform<T>().begin();
	    auto min = *++iter, max = *++iter;
	    if (min > max)
		std::swap(min, max);

	    for (auto elem : take(uniform<T>(min, max), NumberSamples)) {
		EXPECT_GE(elem, min);
		EXPECT_LE(elem, max);
	    }
	});
}

TEST(Costr, Floating)
{
    core::mp::foreach<FloatingTypes>([]<class T>() {
	    for (auto elem : take(uniform<T>(-1.0, +1.0), NumberSamples)) {
		EXPECT_GE(elem, -1.0);
		EXPECT_LE(elem, +1.0);
	    }
	});
}

TEST(Costr, String)
{
    for (auto str : take(str::uniform(uniform<size_t>(0, 20)), NumberSamples))
	EXPECT_LE(str.size(), 20);
    
    for (auto str : take(str::lowercase(uniform<size_t>(0, 20)), NumberSamples)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str) 
	    EXPECT_TRUE(std::isalpha(c) and std::islower(c));
    }
    
    for (auto str : take(str::uppercase(uniform<size_t>(0, 20)), NumberSamples)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str)
	    EXPECT_TRUE(std::isalpha(c) and std::isupper(c));
    }
    
    for (auto str : take(str::alpha(uniform<size_t>(0, 20)), NumberSamples)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str)
	    EXPECT_TRUE(std::isalpha(c));
    }
    
    for (auto str : take(str::alphanum(uniform<size_t>(0, 20)), NumberSamples)) {
	EXPECT_LE(str.size(), 20);
	for (auto c : str)
	    EXPECT_TRUE(std::isalnum(c));
    }
}

TEST(Costr, Pair)
{
    core::mp::foreach<std::tuple<int,real>>([]<class T>() {
	    core::mp::foreach<std::tuple<int,real>>([]<class U>() {
		    auto g = uniform<std::pair<T,U>>(T{0}, T{10}, U{0}, U{10});
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
    auto g0 = uniform<int>(-20, +20);
    auto g1 = uniform<real>(-1.0, +1.0);
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
    auto g0 = uniform<int>(-20, +20);
    auto g1 = uniform<real>(-1.0, +1.0);
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
	    auto g = uniform<T>(0, 10, -20, 20);
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
    auto size = uniform<size_t>(0, 20);
    auto key = uniform<int>(0, 100);
    auto mapped = str::alpha();
    auto value = zip_pair(std::move(key), std::move(mapped));
    auto g = sample<std::map<int,string>>(std::move(size), std::move(value));
    for (auto map : take(std::move(g), NumberSamples)) {
	EXPECT_LE(map.size(), 20);
	for (const auto& [key, value] : map) {
	    EXPECT_GE(key, 0);
	    EXPECT_LE(key, 100);
	    EXPECT_LE(value.size(), 20);
	    for (auto c : value)
		EXPECT_TRUE(std::isalpha(c));
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
	    auto elem_src = uniform<typename T::value_type>(0, 10, -20, 20);
	    auto src = sample<T>(0, 10, std::move(elem_src));
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
    auto gp = uniform<std::pair<int,real>>(-10, +10, -1.0, +1.0);
    auto g = sample<std::vector<std::pair<int,real>>>(std::move(gp));
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
    auto gv0 = uniform<vector<int>>(0, 10, -100, +100);
    auto gv1 = uniform<vector<real>>(0, 5, -1.0, +1.0);
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

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


