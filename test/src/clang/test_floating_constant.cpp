// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/parse.h"
#include "peg/clang/parser.h"
#include "ranges/view.h"

TEST(PegClang, FloatingConstantFailure)
{
    using Parser = peg::clang::FloatingConstant;
    string str = "f1";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(PegClang, FloatingConstantDecSuccess)
{
    using Parser = peg::clang::FloatingConstant;
    string str = "123.4567e+89L";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), str);
}

TEST(PegClang, FloatingConstantHexSuccess)
{
    using Parser = peg::clang::FloatingConstant;
    string str = "0x123.456p1f";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), str);
}

TEST(PegClang, FloatingDecimalConstantGenerator)
{
    const auto NumberSamples = 10'000;
    strings possible_exp = { "e", "E" };
    strings possible_suffix = { "", "f", "F", "l", "L" };
    strings possible_sign = { "", "-", "+" };
    
    auto digit_sequence = v::chr::decimal()
	| v::group_n(v::uniform(1,3))
	| v::subrange_to<string>;
    auto exponent_part = v::zip_cat(v::uniform(possible_exp), v::uniform(possible_sign));
    auto floating_suffix = v::uniform(possible_suffix);
    auto decimal_floating_0 =
	v::zip_cat(digit_sequence, exponent_part, digit_sequence, floating_suffix);
    
    for (auto str : decimal_floating_0 | v::take(NumberSamples))
    {
	using Parser = peg::clang::FloatingConstant;
	auto r = peg::parse<Parser>(str);
	EXPECT_TRUE(r);
	EXPECT_EQ(r.match(), str);
    }
}

TEST(PegClang, FloatingHexConstantGenerator)
{
    const auto NumberSamples = 10;
    strings possible_prefix = { "0x", "0X" };
    strings possible_sign = { "", "-", "+" };
    strings possible_exp = { "p", "P" };
    strings possible_suffix = { "", "f", "F", "l", "L" };

    auto prefix = v::uniform(possible_prefix);
    auto digit_sequence = v::chr::decimal()
	| v::group_n(v::uniform(1,3))
	| v::subrange_to<string>;
    auto hex_sequence = v::chr::hex()
	| v::group_n(v::uniform(1,3))
	| v::subrange_to<string>;
    auto exponent_part = v::zip_cat(v::uniform(possible_exp), v::uniform(possible_sign));
    auto floating_suffix = v::uniform(possible_suffix);
    auto hex_floating_0 =
	v::zip_cat(prefix, hex_sequence, exponent_part, digit_sequence, floating_suffix);
    
    for (auto str : hex_floating_0 | v::take(NumberSamples))
    {
	using Parser = peg::clang::FloatingConstant;
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



