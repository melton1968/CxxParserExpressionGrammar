// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/character.h"
#include "peg/range.h"
#include "peg/repetition.h"
#include "peg/sequence.h"
#include "peg/whitespace.h"

// Numbers
//
namespace peg {
namespace n {

using Digit = Range<'0','9'>;
using Digits = OneOrMore<Digit>;
using MaybeDigits = ZeroOrMore<Digit>;

using HexDigit = Range<'0','9','a','f','A','F'>;
using HexDigits = OneOrMore<HexDigit>;
using MaybeHexDigits = ZeroOrMore<HexDigit>;

using Sign = Choice<c::Plus, c::Minus>;
using MaybeSign = Maybe<Sign>;

using Integer = Sequence<MaybeSign, Digits>;
using MaybeInteger = Maybe<Integer>;
using MaybeDecimalPoint = Maybe<c::DecimalPoint>;

using Rational = Sequence<Integer, Gs, Maybe<c::ForwardSlash, Gs, Integer>>;
using DecimalL = Sequence<MaybeSign, Digits, MaybeDecimalPoint, MaybeDigits>;
using DecimalR = Sequence<MaybeSign, MaybeDigits, MaybeDecimalPoint, Digits>;
using Decimal = Choice<DecimalL, DecimalR>;

using Exponent = Choice<Character<'e'>, Character<'E'>>;
using Real = Sequence<Decimal, Maybe<Exponent, Integer>>;

}; // end ns n
}; // end ns peg
