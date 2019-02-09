// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/must.h"
#include "peg/character.h"
#include "peg/sequence.h"
#include "peg/parse.h"

using namespace peg;

TEST(Peg, MustFailure)
{
    using P0 = Character<'#'>;
    using P1 = Character<'!'>;
    using P2 = Character<'b'>;
    using Parser = Sequence<P0, Must<P1, P2>>;
    string str = "#bash";
    EXPECT_THROW(parse<Parser>(str), std::runtime_error);
}

TEST(Peg, MustSuccess)
{
    using P0 = Character<'#'>;
    using P1 = Character<'!'>;
    using P2 = Character<'b'>;
    using Parser = Sequence<P0, Must<P1, P2>>;
    string str = "#!bash";
    auto r = parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "#!b");
}

TEST(Peg, IfMustFailure)
{
    using P0 = Character<'#'>;
    using P1 = Character<'!'>;
    using P2 = Character<'b'>;
    using Parser = IfMust<P0,P1,P2>;
    string str = "#bash";
    EXPECT_THROW(parse<Parser>(str), std::runtime_error);
}

TEST(Peg, IfMustSuccess)
{
    using P0 = Character<'#'>;
    using P1 = Character<'!'>;
    using P2 = Character<'b'>;
    using Parser = IfMust<P0,P1,P2>;
    string str = "#!bash";
    auto r = parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "#!b");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



