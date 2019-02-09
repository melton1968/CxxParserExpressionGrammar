// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/range.h"
#include "peg/sequence.h"
#include "peg/repetition.h"

namespace peg {

using Alpha = Range<'a','z','A','Z'>;
using AlphaLower = Range<'a','z'>;
using AlphaUpper = Range<'A','Z'>;
using AlphaNum = Range<'a','z','A','Z','0','9'>;

using IdentifierFirst = Range<'a','z','A','Z','_','_'>;
using IdentifierRest = Range<'a','z','A','Z','0','9','_'>;
using Identifier = Sequence<IdentifierFirst, ZeroOrMore<IdentifierRest>>;

}; // end ns peg
