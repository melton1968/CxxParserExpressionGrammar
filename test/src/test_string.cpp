// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/string.h"
#include "peg/parse.h"

namespace peg::s
{
PEG_STR(auto);
};

TEST(Peg, StringFailure)
{
    using Parser = peg::s::_auto;
    string str = "Auto";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, StringSuccess)
{
    using Parser = peg::s::_auto;
    string str = "auto";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), str);
}

TEST(Peg, StringExtraSuccess)
{
    using Parser = peg::s::_auto;
    string str = "autobreak";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "auto");
}

TEST(Peg, StringCaselessFailure)
{
    using Parser = peg::s::c_auto;
    string str = "breakauto";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, StringCaselessSuccess)
{
    using Parser = peg::s::c_auto;
    string str = "Auto";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), str);
}

TEST(Peg, StringCaselessExtraSuccess)
{
    using Parser = peg::s::c_auto;
    string str = "auTobreak";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "auTo");
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



