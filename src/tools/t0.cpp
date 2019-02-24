// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"
#include "core/demangle.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"
#include "peg/cst/action.h"

using namespace peg;

struct Expr;

struct Number : OneOrMore<Range<'0','9'>>
	      , cst::ProtoNode<Number>
	      , cst::DiscardChildren<true>
{};

struct ExprPlus : Seq<Expr, c::Plus, Number>
		, cst::ProtoNode<ExprPlus>
{};

struct ExprMinus : Seq<Expr, c::Minus, Number>
		, cst::ProtoNode<ExprMinus>
{};

struct Expr : LeftRecursion<Or<ExprPlus, ExprMinus, Number>>
		, cst::ProtoNode<Expr>
{};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    peg::expr::apply_rt<peg::expr::printer, std::tuple<Expr>>::apply(cout);
    cout << endl;

    for (auto str : opts.extra())
    {
	peg::cst::Tree cst;
	auto r = parse<Expr, cst::Action>(str, cst);
	cout << "match: " << r.match() << endl;

	assert(cst.nodes.size() == 1);
	assert(cst.nodes.top()->children.size() == 1);
	
	auto& root = cst.nodes.top()->children.back();
	print(root);
    }

    return 0;
}

