// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/keyword.h"

TEST(PegClang, KeywordFailure)
{
    for (auto str : { "Auto", "breaK" })
    {
	using Parser = peg::clang::Keyword;
	auto r = peg::parse<Parser>(str);
	EXPECT_FALSE(r);
	EXPECT_EQ(r.match(), "");
    }
}

TEST(PegClang, KeywordSuccess)
{
    for (auto str : { "auto", "break", "case", "char", "const", "continue", "default",
			 "do", "double", "else", "enum", "extern", "float", "for", "goto", "if",
			 "inline", "int", "long", "register", "restrict", "return", "short",
			 "signed", "sizeof", "static", "struct", "switch", "typedef", "union",
			 "unsigned", "void", "volatile", "while", "_Alignas", "_Alignof",
			 "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary", "_Noreturn",
			 "_Static_assert", "_Thread_local" })
    {
	using Parser = peg::clang::Keyword;
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



