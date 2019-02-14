// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/peg.h"
#include "ranges/view/container.h"
#include "ranges/view/string.h"
#include "ranges/view/take.h"
#include "ranges/view/uniform.h"
#include "ranges/view/zip.h"

TEST(Peg, NumberGenerative)
{
    strings signs{ "", "+", "-" };
    strings dps{ "", "", "." };
    strings exps{ "", "", "", "", "", "", "e", "e+", "e-", "E", "E+", "E-" };
    
    auto gsign = v::element_of(signs);
    auto gleft = v::str::decimal_digits(v::uniform_index(4));
    auto gdp = v::element_of(dps);
    auto gright = v::str::decimal_digits(v::uniform_index(4));
    auto gnum = v::zip(gsign, gleft, gdp, gright);

    for (auto [s, l, d, r] : gnum | v::take(10))
	cout << s << l << d << r << endl;
    
    // auto ge = v::element_of(exps);
    
    // auto gl = gen::decimal(0u, 4u);
    // auto gdp = gen::elementOf(dps);
    // auto gr = gen::decimal(0u, 4u);
    // auto ge= gen::elementOf(exps);
    // auto gn = gen::decimal(1u, 2u);
    // auto gexp = gen::map(gen::tuple(ge, gn), [=](const auto& tup)
    // 					     {
    // 						 auto [e, n] = tup;
    // 						 return e.size() > 0 ? e + n : "";
    // 					     });
    // auto g = gen::cat(gsign, gl, gdp, gr, gexp);

    // size_t nint{0}, ndec{0}, nfp{0};
    // const size_t NumberSamples = 10000;
    // for (index_t i = 0; i < NumberSamples; ++i)
    // {
    // 	auto str = g();

    // 	auto exp_iter = str.find_first_of("eE");
    // 	auto digit_iter = str.find_first_of("0123456789");
	
    // 	auto has_digits = digit_iter != str.npos and digit_iter < exp_iter;
    // 	auto has_exp = exp_iter != str.npos;
    // 	auto has_dp = str.find_first_of(".") != str.npos;

    // 	if (not has_digits)
    // 	    continue;
	
    // 	if (not has_dp and not has_exp)
    // 	{
    // 	    auto r = peg::parse<peg::n::Integer>(str);
    // 	    EXPECT_TRUE(r);
    // 	    EXPECT_EQ(r.match(), str);
    // 	    ++nint;
    // 	}

    // 	if (not has_exp)
    // 	{
    // 	    auto r = peg::parse<peg::n::Decimal>(str);
    // 	    EXPECT_TRUE(r);
    // 	    EXPECT_EQ(r.match(), str);
    // 	    ++ndec;
    // 	}

    // 	auto r = peg::parse<peg::n::Real>(str);
    // 	EXPECT_TRUE(r);
    // 	EXPECT_EQ(r.match(), str);
    // 	++nfp;
    // }

    // EXPECT_GE(nint, 0.2 * NumberSamples);
    // EXPECT_GE(ndec, 0.3 * NumberSamples);
    // EXPECT_GE(nfp, 0.8 * NumberSamples);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



