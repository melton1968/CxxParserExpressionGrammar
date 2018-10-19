// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

struct Success
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    { return input.success(0); }
};

struct Failure
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    { return input.failure(); }
};

struct Bof
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.bof()) return input.success(0);
	else return input.failure();
    }
};

struct Eof
{
    template<template<typename> typename Control,
	     template<typename> typename... Actions,
	     typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.success(0);
	else return input.failure();
    }
};

}; // end peg
