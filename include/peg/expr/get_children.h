// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/mp/mp.h"

namespace peg::expr
{

template<class T>
struct get_children
{ using type = std::tuple<>; };

template<class T>
requires requires(T) { typename T::Children; }
struct get_children<T>
{ using type = typename T::Children; };

template<class T>
using get_children_t = typename get_children<T>::type;

}; // end ns peg::pexpr

