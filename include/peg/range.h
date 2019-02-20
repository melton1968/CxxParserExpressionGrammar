// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

template<char Lo, char Hi = Lo, char... Cs>
struct Range
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() >= Lo and input.peek() <= Hi) return input.success(1);
	
	if constexpr (sizeof...(Cs) == 0) return input.failure();
	else return Range<Cs...>::template match<Control, Actions...>(input, states...);
    }
};

namespace r {

using Printable = Range<' ','~'>;

};

}; // end peg
