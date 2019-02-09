// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/identifier.h"
#include "peg/parse.h"

TEST(Peg, IdentifierFailureWhiteSpace)
{
    using Parser = peg::Identifier;
    string str = " abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, IdentifierFailureNumber)
{
    using Parser = peg::Identifier;
    string str = "7abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, IdentifierSuccessUnderscore)
{
    using Parser = peg::Identifier;
    string str = "_abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "_abc");
}

TEST(Peg, IdentifierSuccessLetter)
{
    using Parser = peg::Identifier;
    string str = "abc_75";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "abc_75");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



