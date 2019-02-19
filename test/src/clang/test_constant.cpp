// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/constant.h"

TEST(PegClang, IntegerConstantFailure)
{
    using Parser = peg::clang::IntegerConstant;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(PegClang, IntegerConstantSuccess)
{
    for (auto str : {"123", "123u", "123ul", "123ull", "123l", "123ll", "123llu", "123lu"})
    {
	using Parser = peg::clang::IntegerConstant;
	auto r = peg::parse<Parser>(str);
	EXPECT_TRUE(r);
	EXPECT_EQ(r.match(), str);
    }
}

TEST(PegClang, HexConstantSuccess)
{
    for (auto str : {"0x123", "0X123u", "0x123ul", "0X123ull",
			 "0x123l", "0X123ll", "0x123llu", "0X123lu"})
    {
	using Parser = peg::clang::IntegerConstant;
	auto r = peg::parse<Parser>(str);
	EXPECT_TRUE(r);
	EXPECT_EQ(r.match(), str);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



