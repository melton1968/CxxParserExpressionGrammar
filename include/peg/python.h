// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "core/peg/peg.h"

namespace peg
{

using PythonBooleanLiteralTrue = Characters<'T','r','u','e'>;
using PythonBooleanLiteralFalse = Characters<'F','a','l','s','e'>;
using PythonBooleanLiteral = Choice<PythonBooleanLiteralTrue,PythonBooleanLiteralFalse>;

using Sign = Choice<Plus,Minus>;
using OptionalSign = Optional<Sign>;
using PythonNumberLiteral = Sequence<OptionalSign,OneOrMore<DecimalDigit>>;
using PythonTupleInner = List<PythonNumberLiteral,Comma,Optional<WhiteSpace>>;
using PythonTuple = Sequence<OpenParen,PythonTupleInner,CloseParen>;

using DoubleStringLiteral = Sequence<DoubleQuote,ZeroOrMore<NotCharacter<'"'>>,DoubleQuote>;
using SingleStringLiteral = Sequence<SingleQuote,ZeroOrMore<NotCharacter<'\''>>,SingleQuote>;
using PythonStringLiteral = Choice<DoubleStringLiteral,SingleStringLiteral>;

using PythonLiteral = Choice<PythonBooleanLiteral, PythonStringLiteral, PythonNumberLiteral,
			     PythonTuple>;

struct PythonDictionaryKey : PythonStringLiteral { };
struct PythonDictionaryValue : PythonLiteral { };
using PythonDictionaryItem = Sequence<PythonDictionaryKey,Colon,PythonDictionaryValue>;
using PythonDictionaryInner = List<PythonDictionaryItem,Comma,Optional<WhiteSpace>>;
using PythonDictionary = Sequence<OpenCurly,PythonDictionaryInner,CloseCurly>;

}; // end peg
