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
	auto p = input.point();
	while (p < input.end() and std::isspace(*p))
	    ++p;
	return input.success(p - input.point());
    }
};

struct RequiredWhiteSpace
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.point();
	while (p < input.end() and std::isspace(*p))
	    ++p;
	if (p == input.point()) return input.failure();
	else return input.success(p - input.point());
    }
};

struct WhiteSpaceThruEndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.point();
	while (p < input.end() and std::isspace(*p))
	{
	    ++p;
	    if (*(p - 1) == '\n')
		break;
	}
	if (p == input.point()) return input.failure();
	else return input.success(p - input.point());
    }
};

struct WhiteSpaceExceptEndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.point();
	while (p < input.end() and std::isspace(*p) and *p != '\n')
	    ++p;
	return input.success(p - input.point());
    }
};

struct RequiredWhiteSpaceExceptEndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.point();
	while (p < input.end() and std::isspace(*p) and *p != '\n')
	    ++p;
	if (p == input.point()) return input.failure();
	else return input.success(p - input.point());
    }
};

using Ws = WhiteSpace;
using ReqWs = RequiredWhiteSpace;
using WsThruEol = WhiteSpaceThruEndOfLine;

using Gs = WhiteSpaceExceptEndOfLine;
using ReqGs = RequiredWhiteSpaceExceptEndOfLine;

}; // end peg
