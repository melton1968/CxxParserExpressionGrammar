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
	else if (input.peek() != C) return input.failure();
	else if constexpr (sizeof...(Cs) == 0) return input.success(1);
	else return Characters<Cs...>::template match<Actions...>(input.success(1), states...);
    }
};

template<auto& S>
struct String
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof())
	    return input.failure();
	
	const char *a = S;
	const char *b = input.loc();
	while (b < input.end() and *a and *a == *b)
	{
	    ++a;
	    ++b;
	}
	if (*a != '\0')
	    return input.failure();
	return input.success(strlen(S));
    }
};

// Convenience Parsers for Character
//
using SpaceChar = Character<' '>;
using TabChar = Character<'\t'>;
using VerticalTabChar = Character<'\v'>;
using LineFeedChar = Character<'\f'>;
using ReturnChar = Character<'\r'>;
using NewlineChar = Character<'\n'>;

using TildeChar = Character<'~'>;
using BackQuoteChar = Character<'`'>;
using ExclamationChar = Character<'!'>;
using AtSignChar = Character<'@'>;
using HashChar = Character<'#'>;
using DollarChar = Character<'$'>;
using PercentChar = Character<'%'>;
using CaretChar = Character<'^'>;
using AmperstandChar = Character<'&'>;
using StarChar = Character<'*'>;
using OpenParenChar = Character<'('>;
using CloseParenChar = Character<')'>;
using UnderscoreChar = Character<'_'>;
using MinusChar = Character<'-'>;
using PlusChar = Character<'+'>;
using EqualChar = Character<'='>;

using OpenCurlyChar = Character<'{'>;
using OpenBracketChar = Character<'['>;
using CloseCurlyChar = Character<'}'>;
using CloseBracketChar = Character<']'>;
using PipeChar = Character<'|'>;
using BackslashChar = Character<'\\'>;

using ColonChar = Character<':'>;
using SemiColonChar = Character<';'>;
using DoubleQuoteChar = Character<'"'>;
using SingleQuoteChar = Character<'\''>;

using LessThanChar = Character<'<'>;
using CommaChar = Character<','>;
using GreaterThanChar = Character<'>'>;
using PeriodChar = Character<'.'>;
using DotChar = Character<'.'>;
using QuestionMarkChar = Character<'?'>;
using ForwardSlashChar = Character<'/'>;
using DivideChar = Character<'/'>;

}; // end ns peg

#define PEG_MAKE_STRING(s)						\
    struct struct_ ## s { constexpr static const char s[] = #s; };	\
    using String_ ## s = peg::String<struct_ ## s::s>;
