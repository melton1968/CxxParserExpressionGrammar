// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

namespace detail::character
{
struct Identity { Input operator()(const Input& input) const { return input; } };
struct Success { Input operator()(const Input& input) const { return input.success(1); } };
struct Failure { Input operator()(const Input& input) const { return input.failure(); } };

template<class Compare>
struct NormalAction
{
    bool compare(char a, char b) const { return Compare{}(a, b); }
    Input success(const Input& input) const { return input.success(1); }
    Input failure(const Input& input) const { return input.failure(); }
};

template<class Compare>
struct InverseAction
{
    bool compare(char a, char b) const { return Compare{}(a, b); }
    Input success(const Input& input) const { return input.failure(); }
    Input failure(const Input& input) const { return input.success(1); }
};

struct AlwaysTrue { bool operator()(char, char) const { return true; } };
struct AlwaysFalse { bool operator()(char, char) const { return false; } };

struct EqualTo
{ bool operator()(char a, char b) const { return a == b; } };

struct NotEqualTo
{ bool operator()(char a, char b) const { return a != b; } };

struct EqualToCaseless
{ bool operator()(char a, char b) const { return std::tolower(a) == std::tolower(b); } };

struct NotEqualToCaseless
{ bool operator()(char a, char b) const { return std::tolower(a) != std::tolower(b); } };

template<class Match, class Next, char C, char... Cs>
struct CharMatcher
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (Match{}.compare(input.peek(), C)) return Match{}.success(input);
	else if constexpr (sizeof...(Cs) == 0) return Match{}.failure(input.failure());
	else return CharMatcher<Match, Next, Cs...>::template
		 match<Actions...>(Next{}(input), states...);
    }
};

template<class Match, char C, char... Cs>
using CharMatcherSet = CharMatcher<Match, Identity, C, Cs...>;

template<class Match, char C, char... Cs>
using CharMatcherSeq = CharMatcher<Match, Success, C, Cs...>;

template<char C, char... Cs>
using Character = CharMatcherSet<NormalAction<EqualTo>, C, Cs...>;

template<char C, char... Cs>
using CharacterCaseless = CharMatcherSet<NormalAction<EqualToCaseless>, C, Cs...>;

using AnyCharacter = CharMatcherSet<NormalAction<AlwaysTrue>, '?'>;

template<char C, char... Cs>
using NotCharacter = CharMatcherSet<InverseAction<EqualTo>, C, Cs...>;

template<char C, char... Cs>
using NotCharacterCaseless = CharMatcherSet<InverseAction<EqualToCaseless>, C, Cs...>;

template<char C, char... Cs>
using String = CharMatcherSeq<InverseAction<NotEqualTo>, C, Cs...>;

template<char C, char... Cs>
using StringCaseless = CharMatcherSeq<InverseAction<NotEqualToCaseless>, C, Cs...>;

}; // end ns detail::character

using detail::character::Character;
using detail::character::CharacterCaseless;
using detail::character::AnyCharacter;
using detail::character::NotCharacter;
using detail::character::NotCharacterCaseless;
using detail::character::String;
using detail::character::StringCaseless;

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

using _e = CharacterCaseless<'e'>;
using _l = CharacterCaseless<'l'>;
using _p = CharacterCaseless<'p'>;
using _u = CharacterCaseless<'u'>;
using _x = CharacterCaseless<'x'>;

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

using _ll = StringCaseless<'l','l'>;

}; // end ns s


}; // end ns peg
