// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<class P, class... Ps>
struct Sequence
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, states...);
	if (not r) return r;
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Sequence<Ps...>::template match<Actions...>(r, states...);
    }
};

template<size_t N, class... Ps>
struct MinSequence
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<Sequence<Ps...>, Actions...>(input, states...);
	if (not r) return r;
	if (r.size() < N) return r.failure();
	return r;
    }
};

template<class P, class... Ps>
struct Choice
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, states...);
	if (r) return r;
	if constexpr (sizeof...(Ps) == 0) return input.failure();
	else return Choice<Ps...>::template match<Actions...>(input, states...);
    }
};

template<class... Ps> using Seq = Sequence<Ps...>;
template<class... Ps> using And = Sequence<Ps...>;
template<class... Ps> using Or = Choice<Ps...>;

}; // end peg
