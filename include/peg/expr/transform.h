// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/mp/mp.h"

namespace peg::expr
{

template<class T, class U>
struct transform_node
{ using type = T; };

template<template<class...> class T, class... Ts, template<class...> class U, class... Us>
struct transform_node<T<Ts...>, U<Us...>>
{ using type = T<Us...>; };

template<class T, class U>
using transform_node_t = typename transform_node<T,U>::type;

}; // end ns peg::pexpr
