// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<typename P>
struct At
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	return input.with_status((bool)r);
    }
};

template<typename P>
struct NotAt
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto r = P::template match<Actions...>(input, states...);
	return input.with_status(not r);
    }
};


}; // end peg
