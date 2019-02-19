// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

// N1570
//
namespace peg::clang
{

// 6.4.3 Universal Character Names
//
struct HexadecimalDigit : Range<'0','9','a','f','A','F'> {};
struct HexQuad: Sequence<HexadecimalDigit, HexadecimalDigit, HexadecimalDigit, HexadecimalDigit>{};
struct UniversalCharacterName4 : Sequence<c::Backslash, Character<'u'>, HexQuad> {};
struct UniversalCharacterName8 : Sequence<c::Backslash, Character<'U'>, HexQuad, HexQuad> {};
struct UniversalCharacterName : Choice<UniversalCharacterName4, UniversalCharacterName8> {}; 

// 6.4.2 Identifiers
//
struct IdentifierDigit : Choice<Range<'a','z','A','Z','_','_','0','9'>, UniversalCharacterName> {};
struct IdentifierNonDigit : Choice<Range<'a','z','A','Z','_','_'>, UniversalCharacterName> {};
struct Identifier : Sequence<IdentifierNonDigit, ZeroOrMore<IdentifierDigit>> {};

}; // end peg::clang
