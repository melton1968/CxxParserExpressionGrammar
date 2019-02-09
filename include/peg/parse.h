// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<typename Parser, template<typename> typename... Actions, typename... States>
Input parse(const Input& input, States&... states)
{
    return Control::template match<Parser, Actions...>(input, states...);
}
    
template<typename Parser, template<typename> typename... Actions, typename... States>
Input parse(const string& str, States&... states)
{
    Input input{ str };
    return Control::template match<Parser, Actions...>(input, states...);
}

}; // end peg
