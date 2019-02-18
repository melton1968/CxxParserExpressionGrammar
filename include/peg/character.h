// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<char C>
struct Character
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() == C) return input.success(1);
	else return input.failure();
    }
};

template<char C>
struct CharacterLower
{
    static_assert(C >= 'a' and C <= 'z');
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (std::tolower(input.peek()) == C) return input.success(1);
	else return input.failure();
    }
};

template<char C>
struct NotCharacter
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() != C) return input.success(1);
	else return input.failure();
    }
};

struct AnyCharacter
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else return input.success(1);
    }
};

template<char C, char... Cs>
struct Characters
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() == C) return input.success(1);
	else if constexpr (sizeof...(Cs) == 0) return input.failure();
	else return Characters<Cs...>::template match<Actions...>(input, states...);
    }
};

template<char C, char... Cs>
struct CharactersLower
{
    static_assert(C >= 'a' and C <= 'z');
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (std::tolower(input.peek()) == C) return input.success(1);
	else if constexpr (sizeof...(Cs) == 0) return input.failure();
	else return CharactersLower<Cs...>::template match<Actions...>(input, states...);
    }
};

template<char C, char... Cs>
struct String
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() != C) return input.failure();
	else if constexpr (sizeof...(Cs) == 0) return input.success(1);
	else return String<Cs...>::template match<Actions...>(input.success(1), states...);
    }
};

template<char C, char... Cs>
struct StringLower
{
    static_assert(C < 'A' or C > 'Z');
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (std::tolower(input.peek()) != C) return input.failure();
	else if constexpr (sizeof...(Cs) == 0) return input.success(1);
	else return StringLower<Cs...>::template match<Actions...>(input.success(1), states...);
    }
};


// Convenience Parsers for Character
//
namespace c {

using _0 = Character<'0'>;
using a = Character<'a'>;
using b = Character<'b'>;
using l = Character<'l'>;
using L = Character<'L'>;
using u = Character<'u'>;
using U = Character<'U'>;

using _e = CharacterLower<'e'>;
using _l = CharacterLower<'l'>;
using _p = CharacterLower<'p'>;
using _u = CharacterLower<'u'>;
using _x = CharacterLower<'x'>;

using Space = Character<' '>;
using Tab = Character<'\t'>;
using VerticalTab = Character<'\v'>;
using LineFeed = Character<'\f'>;
using Return = Character<'\r'>;
using Newline = Character<'\n'>;

using Tilde = Character<'~'>;
using BackQuote = Character<'`'>;
using Exclamation = Character<'!'>;
using Bang = Character<'!'>;
using AtSign = Character<'@'>;
using Hash = Character<'#'>;
using Dollar = Character<'$'>;
using Percent = Character<'%'>;
using Caret = Character<'^'>;
using Amperstand = Character<'&'>;
using Star = Character<'*'>;
using Multiply  = Character<'*'>;
using OpenParen = Character<'('>;
using CloseParen = Character<')'>;
using Underscore = Character<'_'>;
using Minus = Character<'-'>;
using Plus = Character<'+'>;
using Equal = Character<'='>;

using OpenCurly = Character<'{'>;
using OpenBracket = Character<'['>;
using CloseCurly = Character<'}'>;
using CloseBracket = Character<']'>;
using Pipe = Character<'|'>;
using Backslash = Character<'\\'>;

using Colon = Character<':'>;
using SemiColon = Character<';'>;
using DoubleQuote = Character<'"'>;
using SingleQuote = Character<'\''>;

using LessThan = Character<'<'>;
using Comma = Character<','>;
using GreaterThan = Character<'>'>;
using Period = Character<'.'>;
using DecimalPoint = Character<'.'>;
using Dot = Character<'.'>;
using QuestionMark = Character<'?'>;
using ForwardSlash = Character<'/'>;
using Divide = Character<'/'>;

}; // end ns c

namespace s
{

using _ll = StringLower<'l','l'>;

}; // end ns s


}; // end ns peg
