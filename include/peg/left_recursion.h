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
    using Self = LeftRecursion<P>;
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
	(Actions<Self>::begin_recursion(input, states...), ...);
	
	auto longest = input;
	while(true)
	{
	    seeds[begin] = longest.point();
	    
	    auto r = Control::template match<P, Actions...>(input, states...);
	    if (r and r.point() > seeds[begin])
	    {
		longest = r;
		(Actions<Self>::success_recursion(longest, states...), ...);
		seeds[begin] = longest.point();
	    }
	    else
	    {
		(Actions<Self>::failure_recursion(r, states...), ...);
		break;
	    }
	}

	(Actions<LeftRecursion<P>>::end_recursion(longest, states...), ...);
	return longest;
    }
};

}; // end peg
