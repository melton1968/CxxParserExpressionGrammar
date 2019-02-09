// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<class P, class... Ps>
struct Must
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (not r) Control::template raise<P>();
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Must<Ps...>::template match<Actions...>(r, states...);
    }
};

template<class P, class... Ps>
struct IfMust
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (not r) return r;
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Must<Ps...>::template match<Actions...>(r, states...);
    }
};

}; // end peg
