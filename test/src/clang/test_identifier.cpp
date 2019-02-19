// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/identifier.h"

TEST(PegClang, IdentifierFailure)
{
    using Parser = peg::clang::Identifier;
    string str = "0abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(PegClang, IdentifierSuccess)
{
    using Parser = peg::clang::Identifier;
    string str = "_abc7";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), str);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



