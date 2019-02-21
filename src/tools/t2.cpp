// Copyright (C) 2019 by Mark Melton
//

#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "core/mp/mp.h"
#include "core/tuple/apply.h"

struct Or {};
struct And {};
struct Plus {};
struct Minus {};
struct CharA {};
struct CharB {};

using namespace core;

using T0 = mp::list<Or, mp::list<And, mp::list<CharA,Plus,CharB>>, mp::list<CharA,Minus,CharB>>;

template<template<class...> class F, class T>
struct apply;

template<template<class...> class F>
struct apply<F, mp::list<>>
{
    using type = mp::list<>;
};

template<template<class...> class F, class T>
struct apply
{
    using Head = mp::head_t<T>;
    using Tail = mp::tail_t<T>;
    using type = mp::push_front_t<F<Head>, typename apply<F, Tail>::type>;
};

template<class T>
struct foo { using type = T; };

template<class... Ts>
struct foo<core::mp::list<Ts...>>
{ using type = std::tuple<Ts...>; };

using T2 = typename apply<foo, T0>::type;

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    cout << type_name<T0>() << endl;
    cout << type_name<T2>() << endl;
    
    return 0;
}

