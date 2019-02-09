// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/at.h"
#include "peg/character.h"
#include "peg/block.h"
#include "peg/parse.h"

TEST(Peg, BlockFailureStart)
{
    using P0 = peg::Characters<'/', '*'>;
    using P1 = peg::AnyCharacter;
    using P2 = peg::Characters<'*', '/'>;
    using Parser = peg::Block<P0, P1, P2>;
    string str = "abc ABC */";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, BlockFailureStart2)
{
    using P0 = peg::Characters<'/', '*'>;
    using P1 = peg::AnyCharacter;
    using P2 = peg::Characters<'*', '/'>;
    using Parser = peg::Block<P0, P1, P2>;
    string str = "/ *abc ABC */";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, BlockFailureEnd)
{
    using P0 = peg::Characters<'/', '*'>;
    using P1 = peg::AnyCharacter;
    using P2 = peg::Characters<'*', '/'>;
    using Parser = peg::Block<P0, P1, P2>;
    string str = "/* abc ABC * /";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, BlockEndOfFile)
{
    using P0 = peg::Characters<'/', '*'>;
    using P1 = peg::AnyCharacter;
    using P2 = peg::Characters<'*', '/'>;
    using Parser = peg::Block<P0, P1, P2>;
    string str = "";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, BlockSuccess)
{
    using P0 = peg::Characters<'/', '*'>;
    using P1 = peg::AnyCharacter;
    using P2 = peg::Characters<'*', '/'>;
    using Parser = peg::Block<P0, P1, P2>;
    string str = "/* abc \n DEF */ 123";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "/* abc \n DEF */");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



