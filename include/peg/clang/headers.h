// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

namespace peg::clang
{

// 6.4.7 Header names
//
struct QChar : NotCharacter<'"', '\n'> {};
struct QCharSequence : OneOrMore<QChar> {};

struct HChar : NotCharacter<'>', '\n'> {};
struct HCharSequence : OneOrMore<HChar> {};

struct HeaderName : Choice<
    Seq<c::LessThan, HCharSequence, c::GreaterThan>,
    Seq<c::DoubleQuote, QCharSequence, c::DoubleQuote>>
{};

}; // end ns peg
