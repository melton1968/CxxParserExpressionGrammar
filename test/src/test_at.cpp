// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/at.h"
#include "peg/character.h"
#include "peg/parse.h"

TEST(Peg, AtFailure)
{
    using P0 = peg::Character<'b'>;
    using Parser = peg::At<P0>;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, AtEndOfFile)
{
    using P0 = peg::Character<'b'>;
    using Parser = peg::At<P0>;
    string str = "";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, AtSuccess)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::At<P0>;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NotAtFailure)
{
    using P0 = peg::Character<'a'>;
    using Parser = peg::NotAt<P0>;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NotAtEndOfFile)
{
    using P0 = peg::Character<'b'>;
    using Parser = peg::NotAt<P0>;
    string str = "";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NotAtSuccess)
{
    using P0 = peg::Character<'b'>;
    using Parser = peg::NotAt<P0>;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



