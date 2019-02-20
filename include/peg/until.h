// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

template<typename P, typename Q, bool ConsumeP = true>
struct Until
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	Input in = input;
	while (true)
	{
	    auto r = Control::template match<P, Actions...>(in, states...);
	    if (r)
	    {
		if constexpr (ConsumeP)
				 return r;
		else
		    return in;
	    }

	    auto s = Control::template match<Q, Actions...>(in, states...);
	    if (not s) return s;

	    in = s;
	}
    }
};


}; // end peg
