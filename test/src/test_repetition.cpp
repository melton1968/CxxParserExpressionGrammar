// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/character.h"
#include "peg/repetition.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

TEST(Peg, ZeroOrOneSuccess0)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::ZeroOrOne<P0>;
    string str = "xaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, ZeroOrOneSuccess1)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::ZeroOrOne<P0>;
    string str = "aaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, ZeroOrMoreSuccess0)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::ZeroOrMore<P0>;
    string str = "xaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, ZeroOrMoreSuccess1)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::ZeroOrMore<P0>;
    string str = "aaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "aaa");
}

TEST(Peg, ZeroOrMoreSuccess2)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::ZeroOrMore<P0>;
    string str = "aaax";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "aaa");
}

TEST(Peg, OneOrMoreSuccess0)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::OneOrMore<P0>;
    string str = "ab";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, OneOrMoreSuccess1)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::OneOrMore<P0>;
    string str = "aaab";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "aaa");
}

TEST(Peg, OneOrMoreFailure)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::OneOrMore<P0>;
    string str = "xaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



