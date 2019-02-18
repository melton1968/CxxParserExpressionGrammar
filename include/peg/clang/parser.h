// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

// N1570
//
namespace peg::clang
{

// 6.4.4.2 - Floating Constants
//
struct MaybeFloatingSuffix : Maybe<Character<'f', 'l', 'F', 'L'>> {};
struct MaybeSign : Maybe<Character<'-', '+'>> {};
struct Digit : Range<'0','9'> {};
struct DigitSequence : OneOrMore<Digit> {};

struct BinaryExponentPart : Seq<c::_p, MaybeSign, DigitSequence> {};
struct ExponentPart : Seq<c::_e, MaybeSign, DigitSequence> {};

struct HexadecimalDigit : Range<'0','9','a','f','A','F'> {};
struct HexadecimalDigitSequence : OneOrMore<HexadecimalDigit> {};

struct FractionalConstant : Or<
    Seq<Maybe<DigitSequence>, c::DecimalPoint, DigitSequence>,
    Seq<DigitSequence, c::DecimalPoint>>
{};

struct HexadecimalFractionalConstant : Or<
    Seq<Maybe<HexadecimalDigitSequence>, c::DecimalPoint, HexadecimalDigitSequence>,
    Seq<HexadecimalDigitSequence, c::DecimalPoint>>
{};

struct DecimalFloatingConstant : Or<
    Seq<FractionalConstant, Maybe<ExponentPart>, MaybeFloatingSuffix>,
    Seq<DigitSequence, ExponentPart, MaybeFloatingSuffix>>
{};

struct HexadecimalPrefix : StringCaseless<'0','x'> {};
struct HexadecimalFloatingConstant : Or<
    Seq<HexadecimalPrefix, HexadecimalFractionalConstant, Maybe<BinaryExponentPart>,
	MaybeFloatingSuffix>,
    Seq<HexadecimalPrefix, HexadecimalDigitSequence, BinaryExponentPart,
	MaybeFloatingSuffix>>
{};

struct FloatingConstant : Or<DecimalFloatingConstant, HexadecimalFloatingConstant> {};


// 6.4.4.1 - Integer Constants
//
struct NonZeroDigit : Range<'1','9'> {};
struct DecimalConstant : Seq<NonZeroDigit, ZeroOrMore<Digit>> {};

struct OctalDigit : Range<'0','7'> {};
struct OctalConstant : Seq<c::_0, ZeroOrMore<OctalDigit>> {};

struct HexadecimalConstant : Seq<HexadecimalPrefix, OneOrMore<HexadecimalDigit>> {};

struct IntegerSuffixUL : Seq<c::_u, Maybe<c::_l>> {};
struct IntegerSuffixULL : Seq<c::_u, Maybe<s::_ll>> {};
struct IntegerSuffixL :	Seq<c::_l, Maybe<c::_u>> {};
struct IntegerSuffixLL : Seq<s::_ll, Maybe<c::_u>> {};
struct IntegerSuffix : Or<IntegerSuffixUL, IntegerSuffixULL, IntegerSuffixL, IntegerSuffixLL> {};

struct IntegerConstant : Or<
    Seq<DecimalConstant, Maybe<IntegerSuffix>>,
    Seq<OctalConstant, Maybe<IntegerSuffix>>,
    Seq<HexadecimalConstant, Maybe<IntegerSuffix>>>
{};

// Identifiers
//
struct HexQuad: Sequence<HexadecimalDigit, HexadecimalDigit, HexadecimalDigit, HexadecimalDigit>{};
struct UniversalCharacterName4 : Sequence<c::Backslash, Character<'u'>, HexQuad> {};
struct UniversalCharacterName8 : Sequence<c::Backslash, Character<'U'>, HexQuad, HexQuad> {};
struct UniversalCharacterName : Choice<UniversalCharacterName4, UniversalCharacterName8> {}; 

struct IdentifierDigit : Choice<Range<'a','z','A','Z','_','_','0','9'>, UniversalCharacterName> {};
struct IdentifierNonDigit : Choice<Range<'a','z','A','Z','_','_'>, UniversalCharacterName> {};
struct Identifier : Sequence<IdentifierNonDigit, ZeroOrMore<IdentifierDigit>> {};

}; // end peg::clang
