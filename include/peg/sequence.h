// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<typename Parser, typename... Parsers>
struct Sequence
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Machine<Parser,Control,Actions...>::match(input, states...);
	if (not r) return r;
	if constexpr (sizeof...(Parsers) == 0) return r;
	else return Sequence<Parsers...>::template match<Control,Actions...>(r, states...);
    }
};

template<typename Parser, typename... Parsers>
struct Choice
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Machine<Parser,Control,Actions...>::match(input, states...);
	if (r) return r;
	if constexpr (sizeof...(Parsers) == 0) return input.failure();
	else return Choice<Parsers...>::template match<Control,Actions...>(input, states...);
    }
};

template<typename... Ps> using And = Sequence<Ps...>;
template<typename... Ps> using Or = Choice<Ps...>;

}; // end peg
