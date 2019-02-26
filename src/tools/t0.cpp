// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"
#include "core/demangle.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"
#include "peg/cst/action.h"
#include "peg/cst/transform.h"

using namespace peg;

struct Expr;

struct Number : NoSkip<OneOrMore<Range<'0','9'>>>
	      , cst::ProtoNode<Number>
	      , cst::DiscardChildren<true>
{};

struct PlusOp : c::Plus
	      , cst::ProtoNode<PlusOp>
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
using MyAction = cst::Action<Parser, cst::DiscardRedundant<true>>;

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
	    
	    cst::transform::apply<ExprPlus, cst::transform::Infix>(root);
	    cst::transform::apply<ExprMinus, cst::transform::Infix>(root);
	    cst::transform::apply<Grammar, cst::transform::ReplaceWithFirstChild>(root);
	    print(root);
	}
    }

    return 0;
}

