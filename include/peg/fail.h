// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

struct Fail
{
    template<class Control, template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    { return input.failure(); }
};

}; // end peg
