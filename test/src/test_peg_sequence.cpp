// Copyright (C) 2018, 2019 by Mark Melton
//

#include <fmt/format.h>
#include <gtest/gtest.h>
#include "core/common.h"
#include "peg/character.h"
#include "peg/sequence.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

TEST(Peg, SequenceSuccess)
{
    using P0 = peg::Character<'a'>;
    using P1 = peg::Character<'b'>;
    using P2 = peg::Character<'c'>;
    using Parser = peg::Sequence<P0,P1,P2>;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "abc");
}

TEST(Peg, SequenceFailure)
{
    using P0 = peg::Character<'a'>;
    using P1 = peg::Character<'b'>;
    using P2 = peg::Character<'c'>;
    using Parser = peg::Sequence<P0,P1,P2>;
    string str = "axc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, ChoiceSuccess0)
{
    using P0 = peg::Character<'a'>;
    using P1 = peg::Character<'b'>;
    using Parser = peg::Choice<P0,P1>;
    string str = "abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, ChoiceSuccess1)
{
    using P0 = peg::Character<'a'>;
    using P1 = peg::Character<'b'>;
    using Parser = peg::Choice<P0,P1>;
    string str = "bac";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "b");
}

TEST(Peg, ChoiceFailure)
{
    using P0 = peg::Character<'a'>;
    using P1 = peg::Character<'b'>;
    using Parser = peg::Choice<P0,P1>;
    string str = "cab";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



