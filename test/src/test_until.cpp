// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/peg.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

TEST(Peg, UntilEof)
{
    using P0 = peg::AlphaLower;
    using P1 = peg::AlphaUpper;
    using Parser = peg::Until<P1, P0>;
    string str = "aaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}


TEST(Peg, UntilNoMatch)
{
    using P0 = peg::AlphaLower;
    using P1 = peg::AlphaUpper;
    using Parser = peg::Until<P1, P0>;
    string str = "123";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}


TEST(Peg, UntilEmpty)
{
    using P0 = peg::AlphaLower;
    using P1 = peg::AlphaUpper;
    using Parser = peg::Until<P1, P0>;
    string str = "A";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "A");
}


TEST(Peg, UntilSuccess)
{
    using P0 = peg::AlphaLower;
    using P1 = peg::AlphaUpper;
    using Parser = peg::Until<P1, P0>;
    string str = "aaaAAA";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "aaaA");
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



