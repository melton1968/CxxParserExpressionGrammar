// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/comments.h"

TEST(PegClang, CommentLineFailure)
{
    using Parser = peg::clang::LineComment;
    string str = "/ / /* remainder of line\nnext line";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(PegClang, CommentLineSuccess)
{
    using Parser = peg::clang::LineComment;
    string str = "// remainder of line\nnext line";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "// remainder of line");
}

TEST(PegClang, CommentPairedFailure)
{
    using Parser = peg::clang::PairedComment;
    string str = "/* remainder of line\nnext line";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(PegClang, CommentPairedSuccess)
{
    using Parser = peg::clang::PairedComment;
    string str = "/* remainder of line\nnext */ line";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "/* remainder of line\nnext */");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



