// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<char Q = '\'', char E = '\\'>
struct QuotedString
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() != Q) return input.failure();

	const char *ptr = input.point();
	while (true)
	{
	    ++ptr;
	    
	    if (not (ptr < input.end()))
		return input.failure();

	    if (*ptr == Q)
		return input.success(++ptr - input.point());
	    
	    if (*ptr == E)
		++ptr;
	}
    }
};


}; // end peg
