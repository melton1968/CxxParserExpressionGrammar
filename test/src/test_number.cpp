// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/peg.h"
#include "core/generator/boolean.h"
#include "core/generator/string.h"

TEST(Peg, NumberPlusSign)
{
    using Parser = peg::n::Sign;
    string str = "+";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "+");
}

TEST(Peg, NumberMinusSign)
{
    using Parser = peg::n::Sign;
    string str = "-";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "-");
}

TEST(Peg, NumberNoSign)
{
    using Parser = peg::n::Sign;
    string str = "0";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NumberInteger)
{
    using Parser = peg::n::Integer;
    string str = "0123";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "0123");
}

TEST(Peg, NumberPlusInteger)
{
    using Parser = peg::n::Integer;
    string str = "+123";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "+123");
}

TEST(Peg, NumberMinusInteger)
{
    using Parser = peg::n::Integer;
    string str = "-123";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "-123");
}

TEST(Peg, NumberDecimal)
{
    using Parser = peg::n::Decimal;
    string str = "-123.456";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "-123.456");
}

TEST(Peg, NumberDecimalNoFraction)
{
    using Parser = peg::n::Decimal;
    string str = "-123.";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "-123.");
}

TEST(Peg, NumberDecimalNoPoint)
{
    using Parser = peg::n::Decimal;
    string str = "-123";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "-123");
}

TEST(Peg, NumberDecimalFractionOnly)
{
    using Parser = peg::n::Decimal;
    string str = "-.123";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "-.123");
}

TEST(Peg, NumberReal)
{
    using Parser = peg::n::Real;
    string str = "123.456e3";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "123.456e3");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



