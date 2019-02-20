// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "core/demangle.h"
#include "peg/input.h"

namespace peg
{

template<class Parser>
struct DebugAction
{
    using Type = DebugAction<Parser>;
    template<class... States>
    static void apply(const Input& input, States&...)
    {
	auto view = core::type_name<Type>();
	cout << string(view) << ": ";
	cout << input.match() << endl;
    }
};

struct BasicControl
{
    template<class Parser, template<class> class... Actions, class... States>
    static Input match(Input input, States&... states)
    {
	auto p = input.point();
	auto r = Parser::template match<BasicControl, Actions...>(input, states...);
	r.mark(p);
	
	if (not r) return r;
	(Actions<Parser>::apply(r, states...) , ...);
	return r;
    }

    template<class T>
    static void raise()
    {
	throw std::runtime_error("Control::raise");
    }
};

template<class NewControl>
struct SwitchControl
{
    template<class Control, class Parser, template<class> class... Actions, class... States>
    static Input match(Input input, States&... states)
    { return NewControl::template match<Actions...>(input, states...); }
};


}; // end peg
