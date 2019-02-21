// Copyright (C) 2019 by Mark Melton
//

#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "core/mp/mp.h"
#include "core/string/replace.h"
#include "peg/peg.h"

namespace peg::mp
{

template<class T>
struct tree_children
{ using type = std::tuple<>; };

template<class T>
requires requires(T) { typename T::Children; }
struct tree_children<T>
{ using type = typename T::Children; };

template<class T>
using tree_children_t = typename tree_children<T>::type;

template<template<class> class F, class T, size_t Depth = 0, class ExcludeSet = std::tuple<>>
struct tree_apply_rt
{
    using Head = core::mp::head_t<T>;
    using Tail = core::mp::tail_t<T>;
    using NewExcludeSet = core::mp::append_t<ExcludeSet, std::tuple<Head>>;
    using Children = tree_children_t<Head>;

    template<class... Args>
    static void apply(Args&&... args)
    {
	F<Head>::apply(Depth, std::forward<Args>(args)...);
	if constexpr (not core::mp::contains_v<Head, ExcludeSet>
		      and  core::mp::size_v<Children> > 0)
			 tree_apply_rt<F, Children, Depth + 1, NewExcludeSet>::apply(args...);
	
	if constexpr (core::mp::size_v<Tail> > 0)
			 tree_apply_rt<F, Tail, Depth, NewExcludeSet>::apply(args...);
    }
};

template<class P>
struct print_parser
{
    static void apply(size_t depth, std::ostream& os)
    {
	auto str = core::type_name<P>();
	cout << string(4 * depth, ' ');
	os << core::replace_all(str, "peg::", "", "detail::", "", "character::", "") << endl;
    }
};

template<class T, class U>
struct pexpr_transform_node
{ using type = T; };

template<template<class...> class T, class... Ts, template<class...> class U, class... Us>
struct pexpr_transform_node<T<Ts...>, U<Us...>>
{ using type = T<Us...>; };

template<class T, class U>
using pexpr_transform_node_t = typename pexpr_transform_node<T,U>::type;

template<template<class...> class F, class... Ts>
struct tree_apply;

template<template<class...> class F>
struct tree_apply<F>
{ using type = core::mp::list<>; };

template<template<class...> class F, class T, class... Ts>
struct tree_apply<F, T, Ts...>
{
    template<class Tx>
    struct invoke_on_list;

    template<template<class...> class Lx, class... Txs>
    struct invoke_on_list<Lx<Txs...>>
    { using type = typename tree_apply<F, Txs...>::type; };
    
    using children = typename invoke_on_list<tree_children_t<T>>::type;
    using tail = typename tree_apply<F, Ts...>::type;
    using head = pexpr_transform_node_t<F<T>, children>;
    using type = core::mp::push_front_t<head, tail>;
};

}; // end ns peg::mp

using namespace peg;

struct Number : Range<'0','9'> {};
struct Numbers : OneOrMore<Range<'0','9'>> {};

struct Fact;
struct Fact : Or<
    Seq<Fact, c::Multiply, Number>,
    Seq<Fact, c::Divide, Number>,
    Number>
{};

struct Expr;
struct Expr : Or<
    Seq<Expr, c::Plus, Fact>,
    Seq<Expr, c::Minus, Fact>,
    Fact>
{};

template<class T>
struct foo { using type = T; };

template<>
struct foo<Number>
{
    using type = Numbers;
};

template<class T>
using foo_t = typename foo<T>::type;

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    using t = mp::tree_apply<foo_t, Seq<Number,Number>>::type;
    mp::tree_apply_rt<mp::print_parser, t>::apply(cout);

    // using t = mp::tree_apply<foo_t, Expr>::type;
    // using e = core::mp::head_t<t>;
    
    //mp::tree_apply_rt<mp::print_parser, std::tuple<Expr>>::apply(cout);
    // mp::tree_apply_rt<mp::print_parser, t>::apply(cout);

    return 0;
}

