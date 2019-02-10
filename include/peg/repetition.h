// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"
#include "peg/sequence.h"

namespace peg
{

template<class P, class... Ps>
struct ZeroOrOne
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    { return ZeroOrOne<Sequence<P, Ps...>>::template match<Actions...>(input, states...); }
};

template<class P>
struct ZeroOrOne<P>
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (r) return r;
	return input;
    }
};

template<class P, class... Ps>
struct ZeroOrMore
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    { return ZeroOrMore<Sequence<P, Ps...>>::template match<Actions...>(input, states...); }
};
	    
template<class P>
struct ZeroOrMore<P>
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto last_r = input;
	while (auto r = P::template match<Actions...>(last_r, states...))
	    last_r = r;
	return last_r;
    }
};

template<class P, class... Ps>
struct OneOrMore
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    { return OneOrMore<Sequence<P, Ps...>>::template match<Actions...>(input, states...); }
};

template<class P>
struct OneOrMore<P>
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (not r) return r.failure();
	return ZeroOrMore<P>::template match<Actions...>(r, states...);
    }
};

// Convenience Parsers for Repetition
//
template<class... Ps> using Optional = ZeroOrOne<Ps...>;
template<class... Ps> using Maybe = ZeroOrOne<Ps...>;
template<class... Ps> using Asterick = ZeroOrMore<Ps...>;

template<class Parser, class Infix, class Ignore>
using List = Sequence<Parser,ZeroOrMore<Ignore,Infix,Parser>,Maybe<Infix>>;

template<class Parser, class Infix, class Ignore>
using InfixList = Sequence<Parser,ZeroOrMore<Ignore,Infix,Parser>>;

}; // end peg
