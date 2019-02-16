// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/peg.h"
#include "ranges/view.h"

static constexpr size_t NumberSamples = 20;

TEST(Peg, GenerativeIntegers)
{
    auto gints = v::arbitrary<int>()
	| v::str::convert()
	| v::take(NumberSamples);
    for (auto str : gints)
    {
	auto ri = peg::parse<peg::n::Integer>(str);
	EXPECT_TRUE(ri);
	EXPECT_EQ(ri.match(), str);
	
	auto rd = peg::parse<peg::n::Decimal>(str);
	EXPECT_TRUE(rd);
	EXPECT_EQ(rd.match(), str);

	auto rr = peg::parse<peg::n::Real>(str);
	EXPECT_TRUE(rr);
	EXPECT_EQ(rr.match(), str);
    }
}

TEST(Peg, GenerativeDecimals)
{
    auto gints = v::uniform<real>(-1e3, 1e3)
	| v::str::convert("{:f}")
	| v::take(NumberSamples);
    for (auto str : gints)
    {
	cout << str << endl;
	auto rd = peg::parse<peg::n::Decimal>(str);
	EXPECT_TRUE(rd);
	EXPECT_EQ(rd.match(), str);

	auto rr = peg::parse<peg::n::Real>(str);
	EXPECT_TRUE(rr);
	EXPECT_EQ(rr.match(), str);
    }
}

TEST(Peg, GenerativeReals)
{
    auto generator = v::arbitrary<real>()
	| v::str::convert()
	| v::take(NumberSamples);
    for (auto str : generator)
    {
	auto r = peg::parse<peg::n::Real>(str);
	EXPECT_TRUE(r);
	EXPECT_EQ(r.match(), str);
    }
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



