// Copyright (C) 2018, 2019 by Mark Melton
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
	return input.success(p - input.loc());
    }
};

struct RequiredWhiteSpace
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

struct WhiteSpaceThruEndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.loc();
	while (p < input.end() and std::isspace(*p))
	{
	    ++p;
	    if (*(p - 1) == '\n')
		break;
	}
	if (p == input.loc()) return input.failure();
	else return input.success(p - input.loc());
    }
};

struct WhiteSpaceExceptEndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.loc();
	while (p < input.end() and std::isspace(*p) and *p != '\n')
	    ++p;
	return input.success(p - input.loc());
    }
};

struct RequiredWhiteSpaceExceptEndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.loc();
	while (p < input.end() and std::isspace(*p) and *p != '\n')
	    ++p;
	if (p == input.loc()) return input.failure();
	else return input.success(p - input.loc());
    }
};

using Ws = WhiteSpace;
using ReqWs = RequiredWhiteSpace;
using WsThruEol = WhiteSpaceThruEndOfLine;

using Gs = WhiteSpaceExceptEndOfLine;
using ReqGs = RequiredWhiteSpaceExceptEndOfLine;

}; // end peg
