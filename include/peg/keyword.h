// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/at.h"
#include "peg/sequence.h"

namespace peg
{

template<typename P, typename Q>
using Keyword = Sequence<P, NotAt<Q>>;

}; // end peg

