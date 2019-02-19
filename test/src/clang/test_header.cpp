// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/header.h"

TEST(PegClang, HeaderFailure)
{
    using Parser = peg::clang::HeaderName;
    string str = "<abc";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(PegClang, HeaderSuccess)
{
    for (auto str : {"\"abc.h\"", "<abc.h>"})
    {
	using Parser = peg::clang::HeaderName;
	auto r = peg::parse<Parser>(str);
	EXPECT_TRUE(r);
	EXPECT_EQ(r.match(), str);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



