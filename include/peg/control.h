// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "core/demangle.h"
#include "peg/input.h"

namespace peg
{

template<typename Parser>
struct DebugAction
{
    using Type = DebugAction<Parser>;
    static void apply(const Input& input)
    {
	auto view = core::get_name<Type>();
	cout << string(view) << ": ";
	cout << input.match() << endl;
    }
};

struct Control
{
    template<typename Parser, template<typename> typename... Actions, typename... States>
    static Input match(Input input, States&... states)
    {
	auto p = input.loc();
	auto r = Parser::template match<Actions...>(input, states...);
	r.mark(p);
	
	if (not r) return r;
	(Actions<Parser>::apply(r, states...) , ...);
	return r;
    }
};


}; // end peg
