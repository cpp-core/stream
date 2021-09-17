// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/generate.h"
#include "core/json/nljson.h"
#include "core/mp/foreach.h"
#include "core/mp/same.h"
#include "core/tuple/apply.h"
#include "core/tuple/map.h"

static const size_t NumberSamples = 64;

using namespace cogen;

using IntegralTypes = std::tuple<int16,int32,int64,uint16,uint32,uint64>;
using FloatingTypes = std::tuple<float,real>;

TEST(Cogen, Constant)
{
    core::mp::foreach<IntegralTypes>([]<class T>() {
	    auto value = *uniform<T>().begin();
	    auto g = constant(value);
	    for (auto elem : take(std::move(g), NumberSamples))
		EXPECT_EQ(elem, value);
	});
}

TEST(Cogen, Integral)
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

TEST(Cogen, Floating)
{
    core::mp::foreach<FloatingTypes>([]<class T>() {
	    for (auto elem : take(uniform<T>(-1.0, +1.0), NumberSamples)) {
		EXPECT_GE(elem, -1.0);
		EXPECT_LE(elem, +1.0);
	    }
	});
}

TEST(Cogen, Pair)
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

TEST(Cogen, Zip)
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

TEST(Cogen, ZipPair)
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

TEST(Cogen, Vector)
{
    auto g = uniform<std::vector<int>>(0, 10, -20, 20);
    for (auto vec : take(std::move(g), NumberSamples)) {
	EXPECT_LE(vec.size(), 10);
	for (const auto& elem : vec) {
	    EXPECT_GE(elem, -20);
	    EXPECT_LE(elem, +20);
	}
    }

    auto gv = uniform<std::vector<int>>(0, 10, -20, 20);
    auto gvv = sample<std::vector<std::vector<int>>>(0, 10, std::move(gv));
    for (auto vvec : take(std::move(gvv), NumberSamples)) {
	EXPECT_LE(vvec.size(), 10);
	for (const auto& vec : vvec) {
	    EXPECT_LE(vec.size(), 10);
	    for (const auto& elem : vec) {
		EXPECT_GE(elem, -20);
		EXPECT_LE(elem, +20);
	    }
	}
    }
}

TEST(Cogen, List)
{
    auto g = uniform<std::list<int>>(0, 10, -20, 20);
    for (auto vec : take(std::move(g), NumberSamples)) {
	EXPECT_LE(vec.size(), 10);
	for (const auto& elem : vec) {
	    EXPECT_GE(elem, -20);
	    EXPECT_LE(elem, +20);
	}
    }

    auto gv = uniform<std::list<int>>(0, 10, -20, 20);
    auto gvv = sample<std::list<std::list<int>>>(0, 10, std::move(gv));
    for (auto vvec : take(std::move(gvv), NumberSamples)) {
	EXPECT_LE(vvec.size(), 10);
	for (const auto& vec : vvec) {
	    EXPECT_LE(vec.size(), 10);
	    for (const auto& elem : vec) {
		EXPECT_GE(elem, -20);
		EXPECT_LE(elem, +20);
	    }
	}
    }
}

TEST(Cogen, Deque)
{
    auto g = uniform<std::deque<int>>(0, 10, -20, 20);
    for (auto vec : take(std::move(g), NumberSamples)) {
	EXPECT_LE(vec.size(), 10);
	for (const auto& elem : vec) {
	    EXPECT_GE(elem, -20);
	    EXPECT_LE(elem, +20);
	}
    }

    auto gv = uniform<std::deque<int>>(0, 10, -20, 20);
    auto gvv = sample<std::deque<std::deque<int>>>(0, 10, std::move(gv));
    for (auto vvec : take(std::move(gvv), NumberSamples)) {
	EXPECT_LE(vvec.size(), 10);
	for (const auto& vec : vvec) {
	    EXPECT_LE(vec.size(), 10);
	    for (const auto& elem : vec) {
		EXPECT_GE(elem, -20);
		EXPECT_LE(elem, +20);
	    }
	}
    }
}

TEST(Cogen, VectorPair)
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

TEST(Cogen, PairVector)
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



