// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/range.h"
#include "peg/parse.h"

TEST(Peg, SingleRangeSuccess)
{
    using Parser = peg::Range<'a','z'>;
    string str = "m";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "m");
}

TEST(Peg, SingleRangeFailure)
{
    using Parser = peg::Range<'a','z'>;
    string str = "M";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, TwoRangeSuccess)
{
    using Parser = peg::Range<'a','z','A','Z'>;
    string str = "M";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "M");
}

TEST(Peg, TwoRangeFailure)
{
    using Parser = peg::Range<'a','z','A','Z'>;
    string str = "5";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, TwoPlusRangeSuccess)
{
    using Parser = peg::Range<'a','z','A','Z','_'>;
    string str = "_";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "_");
}

TEST(Peg, TwoPlusRangeFailure)
{
    using Parser = peg::Range<'a','z','A','Z','_'>;
    string str = "5";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



