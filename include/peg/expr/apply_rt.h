// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/common.h"
#include "core/demangle.h"
#include "core/mp/mp.h"
#include "core/string/replace.h"
#include "peg/expr/get_children.h"

namespace peg::expr
{

namespace mp = core::mp;

template<template<class> class F, class T, size_t Depth = 0, class ExcludeSet = std::tuple<>>
struct apply_rt
{
    using Head = mp::head_t<T>;
    using Tail = mp::tail_t<T>;
    using NewExcludeSet = mp::append_t<ExcludeSet, std::tuple<Head>>;
    using Children = get_children_t<Head>;

    template<class... Args>
    static void apply(Args&&... args)
    {
	F<Head>::apply(Depth, std::forward<Args>(args)...);
	if constexpr (not mp::contains_v<Head, ExcludeSet> and  mp::size_v<Children> > 0)
			 apply_rt<F, Children, Depth + 1, NewExcludeSet>::apply(args...);
	
	if constexpr (mp::size_v<Tail> > 0)
			 apply_rt<F, Tail, Depth, NewExcludeSet>::apply(args...);
    }
};

template<class P>
struct printer
{
    static void apply(size_t depth, std::ostream& os)
    {
	auto str = core::type_name<P>();
	os << string(4 * depth, ' ');
	os << core::replace_all(str, "peg::", "", "detail::", "", "character::", "") << endl;
    }
};

}; // end ns peg::pexpr
