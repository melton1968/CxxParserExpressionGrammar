// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

template<class P, class... Ps>
struct Must
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, states...);
	if (not r) Control::template raise<P>();
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Must<Ps...>::template match<Control, Actions...>(r, states...);
    }
};

template<class P, class... Ps>
struct IfMust
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, states...);
	if (not r) return r;
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Must<Ps...>::template match<Control, Actions...>(r, states...);
    }
};

}; // end peg
