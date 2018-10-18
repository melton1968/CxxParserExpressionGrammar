// Copyright (C) 2018 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/peg/whitespace.h"
#include "core/peg/parse.h"

using ::testing::StaticAssertTypeEq;

TEST(Peg, WhiteSpaceSuccess)
{
    string str = " \t\nabc";
    auto r = peg::parse<peg::WhiteSpace>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), " \t\n");
}

TEST(Peg, WhiteSpaceFailure)
{
    string str = "a \t\nabc";
    auto r = peg::parse<peg::WhiteSpace>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



