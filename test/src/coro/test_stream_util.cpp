// Copyright 2021 by Mark Melton
//

#include <gtest/gtest.h>
#include "coro/stream/util.h"

using namespace coro;
using ::testing::StaticAssertTypeEq;

TEST(CoroStreamUtil, CompatibleTypeHelper)
{
    using t0 = typename detail::compatible_type_helper<int, true, false, false>::type;
    StaticAssertTypeEq<t0, int&>();
    
    using t0x = typename detail::compatible_type_helper<int, true, true, false>::type;
    StaticAssertTypeEq<t0x, int&>();
    
    using t1 = typename detail::compatible_type_helper<int, true, false, true>::type;
    StaticAssertTypeEq<t1, const int&>();
    
    using t1x = typename detail::compatible_type_helper<int, true, true, true>::type;
    StaticAssertTypeEq<t1x, const int&>();
    
    using t2 = typename detail::compatible_type_helper<int, false, true, false>::type;
    StaticAssertTypeEq<t2, int&&>();
    
    using t3 = typename detail::compatible_type_helper<int, false, true, true>::type;
    StaticAssertTypeEq<t3, const int&&>();
}

TEST(CoroStreamUtil, CompatibleType)
{
    using t0x = compatible_type<int&, int&>;
    EXPECT_TRUE(t0x::l);
    EXPECT_FALSE(t0x::r);
    EXPECT_FALSE(t0x::c);
	
    using t0 = compatible_type_t<int&, int&>;
    StaticAssertTypeEq<t0, int&>();
    
    using t1x = compatible_type<const int&, int&>;
    EXPECT_TRUE(t1x::l);
    EXPECT_FALSE(t1x::r);
    EXPECT_TRUE(t1x::c);
    
    using t1 = compatible_type_t<const int&, int&>;
    StaticAssertTypeEq<t1, const int&>();

    using t2 = compatible_type_t<int&&, int&&>;
    StaticAssertTypeEq<t2, int&&>();
    
    using t3 = compatible_type_t<const int&&, int&&>;
    StaticAssertTypeEq<t3, const int&&>();
    
    using t4 = compatible_type_t<int&, int&&>;
    StaticAssertTypeEq<t4, int&>();
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



