// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include <deque>
#include "coro/stream/stream.h"
#include "core/utility/fixed.h"

using namespace coro;
static const size_t NumberSamples = 5;

TEST(CoroStreamIo, Istream)
{
    auto g = str::alpha() * sampler<int>(0, 10)
	| group()
	| transform([](const auto& vec) {
	    std::string s;
	    for (const auto& str : vec)
		s += str + "\n";
	    return s;
	})
	| take(NumberSamples);
    
    for (const auto& str : g) {
	std::stringstream sin{str}, sout;
	read_lines(sin) | write_lines(sout);
	EXPECT_EQ(sin.str(), sout.str());
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



