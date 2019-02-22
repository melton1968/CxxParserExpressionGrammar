// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/mp/mp.h"
#include "peg/expr/get_children.h"

namespace peg::expr
{

namespace mp = core::mp;

namespace detail
{

template<class L, class P, class T>
struct recursion;

template<class L, class P, template<class...> class M>
struct recursion<L, P, M<>>
{ using type = M<>; };

template<class L, class P, class T>
struct recursion
{
    using head = mp::head_t<T>;
    static constexpr bool recursing = mp::contains_v<head,L>;
    static_assert(not recursing or mp::is_same_template<P,LeftRecursion>::value);
    
    using maybe_children = mp::if_ct<recursing, mp::list<>, expr::get_children_t<head>>;

    using newL = mp::append_t<L, mp::list<head>>;
    using children = typename recursion<newL, head, maybe_children>::type;
    
    using tail = mp::tail_t<T>;
    using newTail = typename recursion<L, P, tail>::type;
    using type = mp::push_front_t<head, tail>;
};

};

template<class T>
using check_recursion_t = typename detail::recursion<mp::list<>, mp::list<>, mp::list<T>>::type;
					  
}; // end ns peg::pexpr
