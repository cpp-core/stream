// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/stream/stream.h"

using namespace coro;

static std::vector<int> vec{1, 2, 3};
static const std::vector<int> cvec{1, 2, 3};

TEST(CoroStreamTypes, Adapt)
{
    auto g = adapt(vec);
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<int&,int>>));
    
    auto h = adapt(cvec);
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&,int>>));
}

TEST(CoroStreamTypes, Alternate)
{
    auto g = adapt(vec) * adapt(vec) | alternate();
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<const int&, int>>));
    
    auto h = adapt(vec) * adapt(cvec) | alternate();
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&, int>>));
}

TEST(CoroStreamTypes, Choose)
{
    auto g = adapt(vec) * adapt(vec) | choose();
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<int&, int>>));
    
    auto h = adapt(cvec) * adapt(cvec) | choose();
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&, int>>));

    // auto h2 = adapt(vec) * adapt(cvec) | choose();
    // EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&, int>>));
}

TEST(CoroStreamTypes, Filter)
{
    auto g = vec | filter([](int n) { return true; });
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<int&, int>>));
    
    auto h = cvec | filter([](int n) { return true; });
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&, int>>));
}

TEST(CoroStreamTypes, Group)
{
    auto g = vec | group(4);
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<std::vector<int>&&>>));
    
    auto h = cvec | group(4);
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<std::vector<int>&&>>));
}

TEST(CoroStreamTypes, GroupTuple)
{
    auto g = vec | group_tuple<2>();
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<std::tuple<int,int>&&>>));
    
    auto h = cvec | group_tuple<2>();
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<std::tuple<int,int>&&>>));
}

TEST(CoroStreamTypes, Iota)
{
    auto g = iota(3, 10, 2);
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<const int&>>));
}

TEST(CoroStreamTypes, Sequence)
{
    auto g =  vec * vec | sequence();
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<int&>>));
    
    auto h =  cvec * cvec | sequence();
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&>>));
    
    // auto h2 =  vec * cvec | sequence();
    // EXPECT_TRUE((std::is_same_v<decltype(h2), Generator<const int&>>));
}

TEST(CoroStreamTypes, Take)
{
    auto g =  vec | take(10);
    EXPECT_TRUE((std::is_same_v<decltype(g), Generator<int&>>));
    
    auto h =  cvec | take(10);
    EXPECT_TRUE((std::is_same_v<decltype(h), Generator<const int&>>));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



