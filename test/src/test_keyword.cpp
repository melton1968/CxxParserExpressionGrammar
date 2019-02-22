// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/string.h"
#include "peg/keyword.h"
#include "peg/range.h"
#include "peg/parse.h"

namespace peg::test
{
PEG_STR(auto);
using KeywordAuto = Keyword<peg::test::_auto, Range<'a','z','A','Z','_','_','0','9'>>;
};

TEST(Peg, KeywordExtraFailure)
{
    using Parser = peg::test::KeywordAuto;
    string str = "autobreak";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, KeywordCaseFailure)
{
    using Parser = peg::test::KeywordAuto;
    string str = "Auto";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, KeywordSuccess)
{
    using Parser = peg::test::KeywordAuto;
    string str = "auto";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), str);
}

TEST(Peg, KeywordExtraSuccess)
{
    using Parser = peg::test::KeywordAuto;
    string str = "auto";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "auto");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



