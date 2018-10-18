// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<typename Parser,
	 template<typename> typename Control = NormalControl,
	 template<typename> typename... Actions,
	 typename... States>
Input parse(const Input& input, States&... states)
{
    return Machine<Parser,Control,Actions...>::match(input, states...);
}
    
template<typename Parser,
	 template<typename> typename Control = NormalControl,
	 template<typename> typename... Actions,
	 typename... States>
Input parse(const std::string& str, States&... states)
{
    Input input{ str };
    return Machine<Parser,Control,Actions...>::match(input, states...);
}

}; // end peg
