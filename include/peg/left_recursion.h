// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

template<class P>
struct LeftRecursion
{
    static constexpr bool IsLeftRecursionParser = true;
    using Children = std::tuple<P>;
    
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	static std::map<const char*, const char*> seeds;
	auto begin = input.point();
	auto iter = seeds.find(begin);
	if (iter != seeds.end())
	    return input.with_status(iter->second != begin, iter->second - begin);

	seeds[begin] = begin;

	auto best = input;
	auto best_seed = begin;
	while (true)
	{
	    auto r = Control::template match<P, Actions...>(input, states...);
	    if (not r or r.point() <= seeds[begin])
		break;

	    best = r;
	    best_seed = seeds[begin];
	    seeds[begin] = r.point();
	}

	seeds[begin] = best_seed;
	// auto r = Control::template match<P, Actions...>(input, states...);
	seeds.erase(begin);
	(Actions<LeftRecursion<P>>::left_recursion_complete(best, states...), ...);
	return best;
    }
};

}; // end peg
