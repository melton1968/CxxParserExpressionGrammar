// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/clang/identifiers.h"

// N1570
//
namespace peg::clang
{

// 6.4.4.1 - Integer Constants
//
struct Digit : Range<'0','9'> {};
struct DigitSequence : OneOrMore<Digit> {};
struct NonZeroDigit : Range<'1','9'> {};

struct DecimalConstant : Seq<NonZeroDigit, ZeroOrMore<Digit>> {};

struct OctalDigit : Range<'0','7'> {};
struct OctalConstant : Seq<c::_0, ZeroOrMore<OctalDigit>> {};

struct HexadecimalPrefix : StringCaseless<'0','x'> {};
struct HexadecimalDigitSequence : OneOrMore<HexadecimalDigit> {};
struct HexadecimalConstant : Seq<HexadecimalPrefix, HexadecimalDigitSequence> {};

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

// 6.4.4.2 - Floating Constants
//
struct MaybeFloatingSuffix : Maybe<Character<'f', 'l', 'F', 'L'>> {};
struct MaybeSign : Maybe<Character<'-', '+'>> {};

struct BinaryExponentPart : Seq<c::_p, MaybeSign, DigitSequence> {};
struct ExponentPart : Seq<c::_e, MaybeSign, DigitSequence> {};

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

struct HexadecimalFloatingConstant : Or<
    Seq<HexadecimalPrefix, HexadecimalFractionalConstant, Maybe<BinaryExponentPart>,
	MaybeFloatingSuffix>,
    Seq<HexadecimalPrefix, HexadecimalDigitSequence, BinaryExponentPart,
	MaybeFloatingSuffix>>
{};

struct FloatingConstant : Or<DecimalFloatingConstant, HexadecimalFloatingConstant> {};

// 6.4.4.3 Enumeration Constants
//
struct EnumerationConstant : Identifier {};

// 6.4.4.4 Character Constants
//
struct HexadecimalEscapeSequence : Seq<String<'\\','x'>, OneOrMore<HexadecimalDigit>> {};
struct OctalEscapeSequence : Seq<Character<'\\'>, Repetition<1,3,OctalDigit>> {};

struct SimpleEscapeSequence : Or<
    String<'\\', '\''>,
    String<'\\', '"'>,
    String<'\\', '?'>,
    String<'\\', '\\'>,
    String<'\\', 'a'>,
    String<'\\', 'b'>,
    String<'\\', 'f'>,
    String<'\\', 'n'>,
    String<'\\', 'r'>,
    String<'\\', 't'>,
    String<'\\', 'v'>>
{};

struct EscapeSequence : Or<
    SimpleEscapeSequence,
    OctalEscapeSequence,
    HexadecimalEscapeSequence,
    UniversalCharacterName>
{};

struct CChar : NotCharacter<'\'', '\\', '\n'> {};
struct CCharSequence : OneOrMore<CChar> {};
struct CCharacterConstant : Or<
    Seq<c::SingleQuote, CCharSequence, c::SingleQuote>,
    Seq<c::L, c::SingleQuote, CCharSequence, c::SingleQuote>,
    Seq<c::u, c::SingleQuote, CCharSequence, c::SingleQuote>,
    Seq<c::U, c::SingleQuote, CCharSequence, c::SingleQuote>>
{};

// 6.4.4 Constants
//
struct Constant : Or<
    IntegerConstant,
    FloatingConstant,
    EnumerationConstant,
    CCharacterConstant>
{};

}; // end peg::clang
