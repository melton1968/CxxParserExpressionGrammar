// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

template<class P>
struct Recurse
{
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
	while (true)
	{
	    auto r = Control::template match<P, Actions...>(input, states...);	    
	    if (r and r.point() > seeds[begin])
	    {
		best = r;
		seeds[begin] = r.point();
	    }
	    else
	    {
		seeds.erase(begin);
		return best;
	    }
	}
    }
};

}; // end peg
