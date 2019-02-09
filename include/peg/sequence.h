// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<typename P, typename... Ps>
struct Sequence
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, states...);
	if (not r) return r;
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Sequence<Ps...>::template match<Actions...>(r, states...);
    }
};

template<typename P, typename... Ps>
struct Choice
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, states...);
	if (r) return r;
	if constexpr (sizeof...(Ps) == 0) return input.failure();
	else return Choice<Ps...>::template match<Actions...>(input, states...);
    }
};

template<typename... Ps> using And = Sequence<Ps...>;
template<typename... Ps> using Or = Choice<Ps...>;

}; // end peg
