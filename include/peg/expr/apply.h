// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/mp/mp.h"
#include "peg/expr/get_children.h"
#include "peg/expr/transform.h"

namespace peg::expr
{

namespace mp = core::mp;

template<template<class...> class F, class... Ts>
struct apply;

template<template<class...> class F>
struct apply<F>
{ using type = mp::list<>; };

template<template<class...> class F, class T, class... Ts>
struct apply<F, T, Ts...>
{
    template<class Tx>
    struct invoke_on_list;

    template<template<class...> class Lx, class... Txs>
    struct invoke_on_list<Lx<Txs...>>
    { using type = typename apply<F, Txs...>::type; };
    
    using children = typename invoke_on_list<get_children_t<T>>::type;
    using tail = typename apply<F, Ts...>::type;
    using head = transform_node_t<F<T>, children>;
    using type = mp::push_front_t<head, tail>;
};

template<template<class...> class F, class... Ts>
using apply_t = typename apply<F,Ts...>::type;


}; // end ns peg::pexpr
