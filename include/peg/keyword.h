// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<typename P, typename Q>
using Keyword = Sequence<P, NotAt<Q>>;

}; // end peg

#define PEG_MAKE_KEYWORD(s)						\
    struct struct_ ## s { constexpr static const char s[] = #s; };	\
    using String_ ## s = String<struct_ ## s::s>;			\
    using Keyword_ ## s = peg::Keyword<String_ ## s, IdentifierRest>;
