// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

struct StartOfFile
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.bof()) return input.success();
	else return input.failure();
    }
};

struct StartOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.bof()) return input.success();
	else if (input.eof()) return input.failure();
	
	const char *ptr = input.point();
	if (*--ptr == '\n') return input.success();
	else return input.failure();
    }
};

struct EndOfFile
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.success();
	else return input.failure();
    }
};

struct EndOfLine
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof())
	    return input.failure();

	const char *ptr = input.point();
	if (*ptr == '\r')
	    ++ptr;
	if (*ptr == '\n')
	    return input.success(++ptr - input.point());
	
	return input.failure();
    }
};

struct EndOfLineOrFile
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof())
	    return input.success();

	const char *ptr = input.point();
	if (*ptr == '\r')
	    ++ptr;
	if (*ptr == '\n')
	    return input.success(++ptr - input.point());
	
	return input.failure();
    }
};

}; // end peg
