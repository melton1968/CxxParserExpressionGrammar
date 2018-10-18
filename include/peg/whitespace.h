// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

struct WhiteSpace
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.loc();
	while (p < input.end() and std::isspace(*p))
	    ++p;
	if (p == input.loc()) return input.failure();
	else return input.success(p - input.loc());
    }
};

using WS = WhiteSpace;

}; // end peg
