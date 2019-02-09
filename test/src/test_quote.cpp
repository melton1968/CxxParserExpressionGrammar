// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/quote.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

TEST(Peg, QuotedStringNoMatch)
{
    using Parser = peg::QuotedString<>;
    string str = "aaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, QuotedStringLeftQuote)
{
    using Parser = peg::QuotedString<>;
    string str = "'aaa";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, QuotedStringSuccess)
{
    using Parser = peg::QuotedString<>;
    string str = "'aaa'";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "'aaa'");
}

TEST(Peg, QuotedStringSuccessEscape)
{
    using Parser = peg::QuotedString<>;
    string str = "'a\\'aa'";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "'a\\'aa'");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



