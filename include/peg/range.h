// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/control.h"

namespace peg
{

template<char Lo, char Hi = Lo, char... Cs>
struct Range
{
    template<template<typename> typename... Actions, typename... States>
    static Input match(const Input& input, States&... states)
    {
	if (input.eof()) return input.failure();
	else if (input.peek() >= Lo and input.peek() <= Hi) return input.success(1);
	
	if constexpr (sizeof...(Cs) == 0) return input.failure();
	else return Range<Cs...>::template match<Actions...>(input, states...);
    }
};

namespace r {

using Printable = Range<' ','~'>;

};

}; // end peg
