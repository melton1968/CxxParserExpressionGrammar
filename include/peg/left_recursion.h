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
	{
	    auto r = input.with_status(iter->second != begin, iter->second - begin);
	    (Actions<Self>::recursion_matched(begin, r, states...), ...);
	    return r;
	}

	seeds[begin] = begin;
	(Actions<Self>::recursion_begin(input, states...), ...);
	
	auto longest = input;
	while(true)
	{
	    seeds[begin] = longest.point();
	    
	    auto r = Control::template match<P, Actions...>(input, states...);
	    if (r and r.point() > seeds[begin])
	    {
		longest = r;
		(Actions<Self>::recursion_success(begin, longest, states...), ...);
		seeds[begin] = longest.point();
	    }
	    else
	    {
		(Actions<Self>::recursion_failure(begin, r, states...), ...);
		break;
	    }
	}

	seeds.clear();
	(Actions<LeftRecursion<P>>::recursion_end(begin, longest, states...), ...);
	return longest;
    }
};

}; // end peg
