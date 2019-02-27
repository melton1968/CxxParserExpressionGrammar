// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/cst/examples/calc1.h"

using namespace peg::cst;

TEST(PegCst, Calc1Sanity)
{
    auto input = "(1 + 2 - 3 + 4 * (5 + 6) - 2) / 7";
    auto output = calculator::parse(input);
    EXPECT_EQ(output, 6);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



