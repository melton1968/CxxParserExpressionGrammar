// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<class P>
struct Must
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (not r) Control::template raise<P>();
	return r;
    }
};

template<class P, class Q>
struct IfMust
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	if (not r) return r;
	auto s = Q::template match<Actions...>(r, states...);
	if (not s) Control::template raise<Q>();
	return s;
    }
};

}; // end peg
