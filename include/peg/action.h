// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/sequence.h"

namespace peg
{

template<class Parser>
struct NullAction
{
    template<class... States> static void start(Input, States&&...) {}
    template<class... States> static void success(Input, States&&...) {}
    template<class... States> static void failure(Input, States&&...) {}
    template<class... States> static bool validate(Input, States&&...) { return true;}
    template<class... States> static void left_recusion_complete(Input, States&&...) {}
};

template<template<class> class NewAction, class P, class... Ps>
struct Action
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = Control::template match<P, NewAction>(input, states...);
	if (not r) return r;
	if constexpr (sizeof...(Ps) == 0) return r;
	else return Sequence<Ps...>::template match<Control, NewAction>(r, states...);
    }
};

}; // end peg
