// Copyright 2021, 2022 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/fp/either.h"
#include "core/fp/option.h"

using namespace core::fp;
using namespace std::literals::string_literals;
using ::testing::StaticAssertTypeEq;

using MyEither = Either<std::string,int>;

TEST(Either, ConstructLeftRight)
{
    auto r = Right{42};
    ASSERT_TRUE(r.is_right());
    EXPECT_EQ(r.get_right(), 42);

    auto l = Left{"error"s};
    ASSERT_TRUE(l.is_left());
    EXPECT_EQ(l.get_left(), "error"s);
}

TEST(Either, ConstructFromLeftRight)
{
    MyEither r{42};
    ASSERT_TRUE(r.is_right());
    EXPECT_EQ(r.get_right(), 42);

    MyEither r1{Right{42}};
    ASSERT_TRUE(r1.is_right());
    EXPECT_EQ(r1.get_right(), 42);

    MyEither l{Left{"error"s}};
    ASSERT_TRUE(l.is_left());
    EXPECT_EQ(l.get_left(), "error"s);
}

TEST(Either, Contains)
{
    MyEither r{42}, l{Left{"error"s}};
    EXPECT_TRUE(r.contains(42));
    EXPECT_FALSE(l.contains(42));
}

TEST(Either, Exists)
{
    MyEither r{42}, l{Left{"error"s}};
    EXPECT_TRUE(r.exists([](int elem) { return elem == 42; }));
    EXPECT_FALSE(r.exists([](int elem) { return elem != 42; }));
    EXPECT_FALSE(l.exists([](int elem) { return true; }));
}

TEST(Either, FilterOrElse)
{
    MyEither r{42}, l{Left{"error"s}};
    auto s = r.filter_or_else([](int elem) { return elem == 42; }, "bad"s);
    ASSERT_TRUE(s.is_right());
    EXPECT_EQ(s.get_right(), 42);

    auto s1 = r.filter_or_else([](int elem) { return elem != 42; }, "bad"s);
    ASSERT_TRUE(s1.is_left());
    EXPECT_EQ(s1.get_left(), "bad"s);

    auto s2 = l.filter_or_else([](int elem) { return true; }, "bad"s);
    ASSERT_TRUE(s2.is_left());
    EXPECT_EQ(s2.get_left(), "bad"s);

    auto s3 = r.filter_or_else([](int elem) { return false; }, "bad"s);
    ASSERT_TRUE(s3.is_left());
    EXPECT_EQ(s3.get_left(), "bad"s);
}

TEST(Either, FlatMap)
{
    MyEither r{21}, l{Left{"error"s}};
    auto s = r.flat_map([](int elem) { return Right{2 * elem}; });
    ASSERT_TRUE(s.is_right());
    EXPECT_EQ(s.get_right(), 42);

    auto s2 = l.flat_map([](int elem) { return Right{2 * elem}; });
    ASSERT_TRUE(s2.is_left());
    EXPECT_EQ(s2.get_left(), "error"s);
}

TEST(Either, Fold)
{
    MyEither r{21}, l{Left{"error"s}};
    auto s = r.fold([](const std::string& s) { return s + "!"s; },
		    [](int elem) { return 2 * elem; });
    ASSERT_TRUE(s.is_right());
    EXPECT_EQ(s.get_right(), 42);

    auto s1 = l.fold([](const std::string& s) { return s + "!"s; },
		    [](int elem) { return 2 * elem; });
    ASSERT_TRUE(s1.is_left());
    EXPECT_EQ(s1.get_left(), "error!"s);
}

TEST(Either, ForAll)
{
    MyEither r{42}, l{Left{"error"s}};
    auto s = r.forall([](int elem) { return elem == 42; });
    EXPECT_TRUE(s);

    auto s1 = r.forall([](int elem) { return elem != 42; });
    EXPECT_FALSE(s1);
    
    auto s2 = l.forall([](int elem) { return true; });
    EXPECT_TRUE(s2);
    
    auto s3 = l.forall([](int elem) { return false; });
    EXPECT_TRUE(s3);
}

TEST(Either, ForEach)
{
    MyEither r{21}, l{Left{"error"s}};
    r.foreach([](int& elem) { elem *= 2; });
    ASSERT_TRUE(r.is_right());
    EXPECT_EQ(r.get_right(), 42);

    l.foreach([](int& elem) { elem *= 2; });
    ASSERT_TRUE(l.is_left());
    EXPECT_EQ(l.get_left(), "error"s);
}

TEST(Either, GetLeft)
{
    Left l{"error"s};
    StaticAssertTypeEq<decltype(l.get_left()), std::string&>();
    StaticAssertTypeEq<decltype(std::as_const(l).get_left()), const std::string&>();
    EXPECT_EQ(l.get_left(), "error");

    StaticAssertTypeEq<decltype(Left{"error"s}.get_left()), std::string&&>();
    EXPECT_EQ(Left{"error"s}.get_left(), "error");
}

TEST(Either, GetRight)
{
    Right r{42};
    StaticAssertTypeEq<decltype(r.get_right()), int&>();
    StaticAssertTypeEq<decltype(std::as_const(r).get_right()), const int&>();
    EXPECT_EQ(r.get_right(), 42);

    StaticAssertTypeEq<decltype(Right{42}.get_right()), int&&>();
    EXPECT_EQ(Right{42}.get_right(), 42);
}

TEST(Either, Map)
{
    MyEither v{21}, e{Left{"error"s}};
    {
	auto r = v.map([](int& elem) { return 2 * elem; });
	ASSERT_TRUE(r);
	EXPECT_EQ(r.get_right(), 42);

	auto s = e.map([](int& elem) { return 2 * elem; });
	ASSERT_FALSE(s);
	EXPECT_EQ(s.get_left(), "error"s);
    }
    {
	auto r = std::as_const(v).map([](const int& elem) { return 2 * elem; });
	ASSERT_TRUE(r);
	EXPECT_EQ(r.get_right(), 42);

	auto s = std::as_const(e).map([](const int& elem) { return 2 * elem; });
	ASSERT_FALSE(s);
	EXPECT_EQ(s.get_left(), "error"s);
    }
    {
	auto r = std::move(v).map([](int&& elem) { return 2 * elem; });
	EXPECT_TRUE(r);
	EXPECT_EQ(r.get_right(), 42);

	auto s = std::move(e).map([](int&& elem) { return 2 * elem; });
	EXPECT_FALSE(s);
	EXPECT_EQ(s.get_left(), "error"s);
    }
}

TEST(Either, OrElse)
{
    MyEither v{21}, e{Left{"error"s}};
    {
	// auto r = v.or_else([]() { return Left{42.0}; });
	// ASSERT_TRUE(r);
	// EXPECT_EQ(r.get_right(), 21);

	// auto s = e.or_else([]() { return Left{42.0}; });
	// ASSERT_FALSE(s);
	// EXPECT_EQ(s.get_left(), 42.0);
	
	// auto s1 = e.or_else([]() { return Right{42}; });
	// ASSERT_TRUe(s1);
	// EXPECT_EQ(s1.get_right(), 42);
    }
    {
	auto r = std::as_const(v).or_else([]() { return MyEither{42}; });
	ASSERT_TRUE(r);
	EXPECT_EQ(r.get_right(), 21);
	
	auto s = std::as_const(e).or_else([]() { return MyEither{42}; });
	ASSERT_TRUE(s);
	EXPECT_EQ(s.get_right(), 42);
    }
    {
	auto r = std::move(v).or_else([]() { return MyEither{42}; });
	ASSERT_TRUE(r);
	EXPECT_EQ(r.get_right(), 21);
	
	auto s = std::move(e).or_else([]() { return MyEither{42}; });
	ASSERT_TRUE(s);
	EXPECT_EQ(s.get_right(), 42);
    }
}

TEST(Either, ToMaybe)
{
    Right r{42};
    auto r0 = r.to_option();
    ASSERT_TRUE(r0);
    EXPECT_EQ(r0.get(), 42);

    Left l{"error"s};
    auto r1 = l.to_option();
    EXPECT_FALSE(r1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
