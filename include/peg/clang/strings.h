// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

namespace peg
{

// 6.4.5 String literals
//
struct SChar : NotCharacter<'"', '\\', '\n'> {};
struct SCharSequence : OneOrMore<SChar> {};
struct EncodingPrefix : Or<String<'u','8'>, c::u, c::U, c::L> {};
struct StringLiteral : Seq<
    Maybe<EncodingPrefix>,
    c::DoubleQuote,
    Maybe<SCharSequence>,
    c::DoubleQuote>
{};

}; // end ns peg
