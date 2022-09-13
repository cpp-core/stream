// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/option.h"
#include "core/fp/either.h"
#include "core/fp/match.h"

using namespace core::fp;
using namespace std::literals::string_literals;

TEST(Option, Construct)
{
    Some s{42};
    ASSERT_TRUE(s);
    EXPECT_EQ(s.get(), 42);

    None n{};
    EXPECT_FALSE(n);
}

TEST(Option, HasValue)
{
    Some s{42};
    ASSERT_TRUE(s);
    EXPECT_TRUE(s.has_value());

    None n{};
    ASSERT_FALSE(n);
    EXPECT_FALSE(n.has_value());
}

TEST(Option, Contains)
{
    Some s{42};
    EXPECT_TRUE(s.contains(42));
    EXPECT_FALSE(s.contains(21));

    None<int> n{};
    EXPECT_FALSE(n.contains(42));
}

TEST(Option, Exists)
{
    Some s{42};
    EXPECT_TRUE(s.exists([](int elem) { return elem == 42; }));
    EXPECT_FALSE(s.exists([](int elem) { return elem != 42; }));

    None<int> n{};
    EXPECT_FALSE(n.exists([](int elem) { return elem == 42; }));
}

TEST(Option, Filter)
{
    Some s{42};
    auto r0 = s.filter([](int elem) { return elem == 42; });
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);
    
    auto r1 = s.filter([](int elem) { return elem != 42; });
    EXPECT_FALSE(r1);

    None<int> n{};
    auto s2 = n.filter([](int elem) { return elem == 42; });
    EXPECT_FALSE(s2);
}

TEST(Option, FlatMap)
{
    Some s{21};
    auto r0 = s.flat_map([](int elem) { return Option{Some{2 * elem}}; });
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);

    auto r1 = s.flat_map([](int elem) { return Option{None<int>{}}; });
    EXPECT_FALSE(r1);

    None<int> n{};
    auto s2 = n.flat_map([](int elem) { return Option{Some{2 * elem}}; });
    EXPECT_FALSE(s2);
}

TEST(Option, Fold)
{
    Some s{21};
    auto r0 = s.fold([](){ return 21; }, [](int elem){ return 2 * elem; });
    EXPECT_EQ(r0, 42);

    None<int> n{};
    auto r1 = n.fold([](){ return 21; }, [](int elem){ return 2 * elem; });
    EXPECT_EQ(r1, 21);
}

TEST(Option, ForAll)
{
    Some s{42};
    EXPECT_TRUE(s.forall([](int elem) { return elem == 42; }));
    EXPECT_FALSE(s.forall([](int elem) { return elem != 42; }));

    None<int> n{};
    EXPECT_TRUE(n.forall([](int elem) { return false; }));
}

TEST(Option, ForEach)
{
    Some s{21};
    s.foreach([](int& elem) { elem *= 2; });
    ASSERT_TRUE(s);
    EXPECT_EQ(s.get(), 42);

    None<int> n{};
    n.foreach([](int& elem) { elem *= 2; });
    EXPECT_FALSE(n);
}

TEST(Option, GetOrElse)
{
    Some s{42};
    EXPECT_EQ(s.get_or_else(21), 42);

    None<int> n{};
    EXPECT_EQ(n.get_or_else(42), 42);
}

TEST(Option, Map)
{
    Some s{21};
    auto r0 = s.map([](int elem) { return 2 * elem; });
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);

    None<int> n{};
    auto r1 = n.map([](int elem) { return 2 * elem; });
    EXPECT_FALSE(r1);
}

TEST(Option, Match)
{
    Some s{42};
    auto r0 = match(s,
		   [](const OptionSome<int>& some) { return some.get(); },
		   [](const OptionNone<int>& none) { return 0; });
    EXPECT_EQ(r0, 42);

    None<int> n{};
    auto r1 = match(n,
		    [](const OptionSome<int>& some) { return some.get(); },
		    [](const OptionNone<int>& none) { return 0; });
    EXPECT_EQ(r1, 0);
}

TEST(Option, OrElse)
{
    Some s{21};
    auto r0 = s.or_else([]() { return Option{Some{42}}; });
    EXPECT_TRUE(r0);
    EXPECT_EQ(r0.get(), 21);
    
    None<int> n{};
    auto r1 = n.or_else([]() { return Option{Some{42}}; });
    EXPECT_TRUE(r1);
    EXPECT_EQ(r1.get(), 42);
}

TEST(Option, ToLeft)
{
    Some s{"error"s};
    auto l = s.to_left(42);
    ASSERT_TRUE(l.is_left());
    EXPECT_EQ(l.get_left(), "error"s);

    None n{};
    auto l1 = n.to_left(42);
    ASSERT_TRUE(l1.is_right());
    EXPECT_EQ(l1.get_right(), 42);
}

TEST(Option, ToRight)
{
    Some s{"error"s};
    auto r = s.to_right(42);
    ASSERT_TRUE(r.is_right());
    EXPECT_EQ(r.get_right(), "error"s);

    None n{};
    auto r1 = n.to_right(42);
    ASSERT_TRUE(r1.is_left());
    EXPECT_EQ(r1.get_left(), 42);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
