// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"
#include "peg/sequence.h"

namespace peg
{

template<class P>
struct ZeroOrOne
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (r) return r;
	return input;
    }
};

template<class P>
struct ZeroOrMore
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

template<class P>
struct OneOrMore
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
template<class P> using Optional = ZeroOrOne<P>;
template<class P> using Asterick = ZeroOrMore<P>;

template<class Parser, class Infix, class Ignore>
using List = Sequence<Parser,ZeroOrMore<Sequence<Ignore,Infix,Parser>>,Optional<Infix>>;

template<class Parser, class Infix, class Ignore>
using InfixList = Sequence<Parser,ZeroOrMore<Sequence<Ignore,Infix,Parser>>>;

}; // end peg
