// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<class Parser, template<class> class... Actions, class... States>
Input parse(const Input& input, States&... states)
{
    return BasicControl<>::template match<Parser, Actions...>(input, states...);
}
    
template<class Parser, template<class> class... Actions, class... States>
Input parse(const string& str, States&... states)
{
    Input input{ str };
    return BasicControl<>::template match<Parser, Actions...>(input, states...);
}

template<class Control, class Parser, template<class> class... Actions, class... States>
Input parse_with_control(const string& str, States&... states)
{
    Input input{ str };
    return Control::template match<Parser, Actions...>(input, states...);
}

}; // end peg
