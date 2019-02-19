// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/expression.h"

TEST(PegClang, ExpressionFailure)
{
    // using Parser = peg::clang::Expression;
    // string str = "1+1";
    // auto r = peg::parse<Parser>(str);
    // EXPECT_FALSE(r);
    // EXPECT_EQ(r.match(), "");
}

TEST(PegClang, ExpressionSuccess)
{
    // for (auto str : {"1"})
    // {
    // 	using Parser = peg::clang::Expression;
    // 	auto r = peg::parse<Parser>(str);
    // 	EXPECT_TRUE(r);
    // 	EXPECT_EQ(r.match(), str);
    // }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



