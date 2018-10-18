// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"
#include "peg/sequence.h"

namespace peg
{

template<typename Parser>
struct ZeroOrOne
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Machine<Parser,Control,Actions...>::match(input, states...);
	if (r) return r;
	return input;
    }
};

template<typename Parser>
struct ZeroOrMore
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto last_r = input;
	while (auto r = Machine<Parser,Control,Actions...>::match(last_r, states...))
	    last_r = r;
	return last_r;
    }
};

template<typename Parser>
struct OneOrMore
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Machine<Parser,Control,Actions...>::match(input, states...);
	if (not r) return r.failure();
	return ZeroOrMore<Parser>::template match<Control,Actions...>(r, states...);
    }
};

// Convenience Parsers for Repetition
//
template<typename P> using Optional = ZeroOrOne<P>;
template<typename P> using Star = ZeroOrMore<P>;

template<typename Parser, typename Infix, typename Ignore>
using List = Sequence<Parser,ZeroOrMore<Sequence<Ignore,Infix,Parser>>,Optional<Infix>>;

}; // end peg
