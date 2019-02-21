// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/expr/apply_rt.h"

using namespace peg;

template<class P>
struct Collector { static void apply(size_t depth) {} };

TEST(PegExpr, ApplyRT)
{
    struct Childless { };
    struct Parent { using Children = std::tuple<Childless>; };
    expr::apply_rt<Collector, std::tuple<Parent>>::apply();
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



