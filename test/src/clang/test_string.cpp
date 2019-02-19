// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/string.h"

TEST(PegClang, StringLiteralFailure)
{
    for (auto str : { "\"abc", "abc\"", "\"ab\nc\"", "\"ab\\c\"" })
    {
	using Parser = peg::clang::StringLiteral;
	auto r = peg::parse<Parser>(str);
	EXPECT_FALSE(r);
	EXPECT_EQ(r.match(), "");
    }
}

TEST(PegClang, StringLiteralSuccess)
{
    for (auto str : { "\"abc\"", "u8\"abc\"", "u\"abc\"", "U\"abc\"", "L\"abc\"" })
    {
	using Parser = peg::clang::StringLiteral;
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



