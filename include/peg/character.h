// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<char C>
struct Character
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
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
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() != C) return input.success(1);
	else return input.failure();
    }
};

template<char C, char... Cs>
struct Characters
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() != C) return input.failure();
	else if constexpr (sizeof...(Cs) == 0) return input.success(1);
	else return Characters<Cs...>::template match<Control,Actions...>
		 (input.success(1), states...);
    }
};

template<auto& S>
struct String
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
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

template<char Lo, char Hi, char... Cs>
struct Ranges
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() >= Lo and input.peek() <= Hi) return input.success(1);
	
	if constexpr (sizeof...(Cs) == 0) return input.failure();
	else return Ranges<Cs...>::template match<Control,Actions...>(input, states...);
    }
};

template<char Lo, char Hi>
using Range = Ranges<Lo,Hi>;

// Convenience Parsers for Character
//
using OpenParen = Character<'('>;
using CloseParen = Character<')'>;
using OpenCurly = Character<'{'>;
using CloseCurly = Character<'}'>;
using Underscore = Character<'_'>;
using Plus = Character<'+'>;
using Minus = Character<'-'>;
using Times = Character<'*'>;
using Divide = Character<'/'>;
using Comma = Character<','>;
using Colon = Character<':'>;
using DoubleQuote = Character<'"'>;
using SingleQuote = Character<'\''>;

// Convenience Parsers for Range(s)
//
using LowerCase = Range<'a','z'>;
using UpperCase = Range<'A','Z'>;
using Alpha = Ranges<'a','z','A','Z'>;
using AlphaNum = Ranges<'a','z','A','Z','0','9'>;
using DecimalDigit = Range<'0','9'>;
using HexDigit = Ranges<'0','9','a','f','A','F'>;

}; // end peg
