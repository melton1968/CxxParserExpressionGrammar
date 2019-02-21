// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/expr/get_children.h"

using namespace peg;

TEST(PegExpr, GetChildrenNone)
{
    struct Childless { };
    using Children = expr::get_children_t<Childless>;
    static_assert(std::is_same_v<Children, std::tuple<>>);
    EXPECT_EQ(Children{}, std::tuple<>{});
}

TEST(PegExpr, GetChildrenSome)
{
    struct Parent { using Children = std::tuple<int,char>; };
    using Children = expr::get_children_t<Parent>;
    static_assert(std::is_same_v<Children, std::tuple<int,char>>);
    EXPECT_EQ(Children{}, (std::tuple<int,char>{}));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



