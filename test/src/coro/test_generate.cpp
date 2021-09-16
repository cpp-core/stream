// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/generate.h"
#include "core/json/nljson.h"
#include "core/mp/foreach.h"

static const size_t NumberSamples = 64;

using namespace cogen;

using IntegralTypes = std::tuple<int16,int32,int64,uint16,uint32,uint64>;
using FloatingTypes = std::tuple<float,real>;

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
		    for (auto elem : take(uniform<std::pair<T,U>>(T{0}, T{10}, U{0}, U{10}), NumberSamples)) {
			EXPECT_GE(elem.first, 0);
			EXPECT_LE(elem.first, 10);
			EXPECT_GE(elem.second, 0);
			EXPECT_LE(elem.second, 10);
		    }
		});
	});
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



