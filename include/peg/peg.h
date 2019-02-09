// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/action.h"
#include "peg/at.h"
#include "peg/block.h"
#include "peg/character.h"
#include "peg/control.h"
#include "peg/file.h"
#include "peg/input.h"
#include "peg/parse.h"
#include "peg/quote.h"
#include "peg/repetition.h"
#include "peg/range.h"
#include "peg/sequence.h"
#include "peg/until.h"
#include "peg/whitespace.h"

// Convenience Parsers
//
namespace peg
{

// General
//
using Alpha = Range<'a','z','A','Z'>;
using AlphaLower = Range<'a','z'>;
using AlphaUpper = Range<'A','Z'>;
using AlphaNum = Range<'a','z','A','Z','0','9'>;

using Digit = Range<'0','9'>;
using Digits = OneOrMore<Digit>;

using HexDigit = Range<'0','9','a','f','A','F'>;
using HexDigits = OneOrMore<HexDigit>;

using IdentifierFirst = Range<'a','z','A','Z'>;
using IdentifierRest = Range<'a','z','A','Z','0','9','_'>;
using Identifier = Sequence<IdentifierFirst, ZeroOrMore<IdentifierRest>>;

using PrintableChar = Range<' ','~'>;

// Numbers
//
using Sign = Choice<Plus,Minus>;
using Integer = Sequence<Optional<Sign>, OneOrMore<Digit>>;
using DecimalLeft = Sequence<Integer, Period, ZeroOrMore<Digit>>;
using DecimalRight = Sequence<Optional<Sign>, Period, OneOrMore<Digit>>;
using Decimal = Choice<DecimalLeft, DecimalRight>;
using Rational = Sequence<Integer, ForwardSlash, Integer>;
using Exponent = Choice<Character<'e'>, Character<'E'>>;
using Real = Sequence<Decimal, Exponent, Integer>;


}; // end peg
