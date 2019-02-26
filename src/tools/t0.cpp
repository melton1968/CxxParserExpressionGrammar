// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"
#include "core/demangle.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"
#include "peg/cst/action.h"

using namespace peg;

struct InfixTransform
{
    static cst::Node::Ptr transform(cst::Node::Ptr n)
    {
	ExpectEQ(n->children.size(), 3);

	auto new_node = std::move(n->children[1]);
	new_node->children.emplace_back(std::move(n->children[0]));
	new_node->children.emplace_back(std::move(n->children[2]));
	
	EnsureEQ(new_node->children.size(), 2);
	return new_node;
    }
};

template<class T>
struct HasTransform
{ static constexpr bool value = false; };

template<class T>
requires requires(T& t) {{ T::transform(t) } -> cst::Node::Ptr }
struct HasTransform<T>
{ static constexpr bool value = true; };


// cst::Node::Ptr apply_transform(cst::Node::Ptr n)
// {
    
//     for (auto& child : n->children)
// 	child = apply_transform(std::move(child));
//     if constexpr (HasTransform<T>::value)
// 		     n = n::element_type::transform(std::move(n));
//     return n;
// }

struct Expr;

struct Number : NoSkip<OneOrMore<Range<'0','9'>>>
	      , cst::ProtoNode<Number>
	      , cst::DiscardChildren<true>
{};

struct PlusOp : c::Plus
	      , cst::ProtoNode<PlusOp>
	      , InfixTransform
{};

struct MinusOp : c::Minus
	      , cst::ProtoNode<MinusOp>
{};

struct ExprPlus : Seq<Expr, PlusOp, Number>
		, cst::ProtoNode<ExprPlus>
{};

struct ExprMinus : Seq<Expr, MinusOp, Number>
		, cst::ProtoNode<ExprMinus>
{};

struct Expr : LeftRecursion<Or<ExprPlus, ExprMinus, Number>>
	    , cst::ProtoNode<Expr>
{};

struct Grammar : SkipWhiteSpace<Seq<Expr, Must<EndOfFile>>>
	       , cst::ProtoNode<Grammar>
{};

template<class Parser>
using MyAction = cst::Action<Parser, cst::DiscardRedundant<false>>;

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    cout << "node: " << sizeof(cst::Node) << endl;
    cout << "number: " << sizeof(Number) << endl;
    peg::expr::apply_rt<peg::expr::printer, std::tuple<Expr>>::apply(cout);
    cout << endl;

    for (auto str : opts.extra())
    {
	peg::cst::Tree cst;
	auto r = parse<Grammar, MyAction>(str, cst);
	cout << "match: " << r.match() << endl;

	assert(cst.nodes.size() == 1);
	assert(cst.nodes.top()->children.size() <= 1);

	if (cst.nodes.top()->children.size() == 1)
	{
	    auto& root = cst.nodes.top()->children.back();
	    print(root);
	    cout << endl;

	    // root = apply_transform(std::move(root));
	    // print(root);
	}
    }

    return 0;
}

