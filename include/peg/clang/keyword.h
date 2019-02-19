// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"

// N1570
//
namespace peg::clang
{

using KeywordAuto = String<'a', 'u', 't', 'o'>;
using KeywordBreak = String<'b', 'r', 'e', 'a', 'k'>;
using KeywordCase = String<'c', 'a', 's', 'e'>;
using KeywordChar = String<'c', 'h', 'a', 'r'>;
using KeywordConst = String<'c', 'o', 'n', 's', 't'>;
using KeywordContinue = String<'c', 'o', 'n', 't', 'i', 'n', 'u', 'e'>;
using KeywordDefault = String<'d', 'e', 'f', 'a', 'u', 'l', 't'>;
using KeywordDo = String<'d', 'o'>;
using KeywordDouble = String<'d', 'o', 'u', 'b', 'l', 'e'>;
using KeywordElse = String<'e', 'l', 's', 'e'>;
using KeywordEnum = String<'e', 'n', 'u', 'm'>;
using KeywordExtern = String<'e', 'x', 't', 'e', 'r', 'n'>;
using KeywordFloat = String<'f', 'l', 'o', 'a', 't'>;
using KeywordFor = String<'f', 'o', 'r'>;
using KeywordGoto = String<'g', 'o', 't', 'o'>;
using KeywordIf = String<'i', 'f'>;
using KeywordInline = String<'i', 'n', 'l', 'i', 'n', 'e'>;
using KeywordInt = String<'i', 'n', 't'>;
using KeywordLong = String<'l', 'o', 'n', 'g'>;
using KeywordRegister = String<'r', 'e', 'g', 'i', 's', 't', 'e', 'r'>;
using KeywordRestrict = String<'r', 'e', 's', 't', 'r', 'i', 'c', 't'>;
using KeywordReturn = String<'r', 'e', 't', 'u', 'r', 'n'>;
using KeywordShort = String<'s', 'h', 'o', 'r', 't'>;
using KeywordSigned = String<'s', 'i', 'g', 'n', 'e', 'd'>;
using KeywordSizeof = String<'s', 'i', 'z', 'e', 'o', 'f'>;
using KeywordStatic = String<'s', 't', 'a', 't', 'i', 'c'>;
using KeywordStruct = String<'s', 't', 'r', 'u', 'c', 't'>;
using KeywordSwitch = String<'s', 'w', 'i', 't', 'c', 'h'>;
using KeywordTypedef = String<'t', 'y', 'p', 'e', 'd', 'e', 'f'>;
using KeywordUnion = String<'u', 'n', 'i', 'o', 'n'>;
using KeywordUnsigned = String<'u', 'n', 's', 'i', 'g', 'n', 'e', 'd'>;
using KeywordVoid = String<'v', 'o', 'i', 'd'>;
using KeywordVolatile = String<'v', 'o', 'l', 'a', 't', 'i', 'l', 'e'>;
using KeywordWhile = String<'w', 'h', 'i', 'l', 'e'>;
using KeywordAlignas = String<'_', 'A', 'l', 'i', 'g', 'n', 'a', 's'>;
using KeywordAlignof = String<'_', 'A', 'l', 'i', 'g', 'n', 'o', 'f'>;
using KeywordAtomic = String<'_', 'A', 't', 'o', 'm', 'i', 'c'>;
using KeywordBool = String<'_', 'B', 'o', 'o', 'l'>;
using KeywordComplex = String<'_', 'C', 'o', 'm', 'p', 'l', 'e', 'x'>;
using KeywordGeneric = String<'_', 'G', 'e', 'n', 'e', 'r', 'i', 'c'>;
using KeywordImaginary = String<'_', 'I', 'm', 'a', 'g', 'i', 'n', 'a', 'r', 'y'>;
using KeywordNoReturn = String<'_', 'N', 'o', 'r', 'e', 't', 'u', 'r', 'n'>;
using KeywordStaticAssert = String<'_', 'S', 't', 'a', 't', 'i', 'c', '_', 'a', 's', 's', 'e', 'r', 't'>;
using KeywordThreadLocal = String<'_', 'T', 'h', 'r', 'e', 'a', 'd', '_', 'l', 'o', 'c', 'a', 'l'>;

// 6.4.1 Keywords
//
struct Keyword : Or<
    KeywordAuto,
    KeywordBreak,
    KeywordCase,
    KeywordChar,
    KeywordConst,
    KeywordContinue,
    KeywordDefault,
    KeywordDouble,
    KeywordDo,
    KeywordElse,
    KeywordEnum,
    KeywordExtern,
    KeywordFloat,
    KeywordFor,
    KeywordGoto,
    KeywordIf,
    KeywordInline,
    KeywordInt,
    KeywordLong,
    KeywordRegister,
    KeywordRestrict,
    KeywordReturn,
    KeywordShort,
    KeywordSigned,
    KeywordSizeof,
    KeywordStatic,
    KeywordStruct,
    KeywordSwitch,
    KeywordTypedef,
    KeywordUnion,
    KeywordUnsigned,
    KeywordVoid,
    KeywordVolatile,
    KeywordWhile,
    KeywordAlignas,
    KeywordAlignof,
    KeywordAtomic,
    KeywordBool,
    KeywordComplex,
    KeywordGeneric,
    KeywordImaginary,
    KeywordNoReturn,
    KeywordStaticAssert,
    KeywordThreadLocal>
{};

}; // end peg::clang
