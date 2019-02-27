// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"
#include "core/demangle.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"
#include "peg/cst/action.h"
#include "peg/cst/transform.h"

using namespace peg;

struct Number : NoSkip<OneOrMore<Range<'0','9'>>> {};
struct FactorOp : Or<c::Multiply, c::Divide> {};
struct TermOp : Or<c::Plus, c::Minus> {};

// struct Expression;
// struct ParenExpression : Seq<c::OpenParen, Expression, c::CloseParen> {};
// struct Factor : Or<Number, ParenExpression> {};
// struct Term : Seq<Factor, FactorOp, Term> {};
// struct Expression : Or<Seq<Term, TermOp, Expression>, Term> {};
// struct Grammar : SkipWhiteSpace<Seq<Expression, Must<EndOfFile>>> {};

struct Expression;
struct Factor : Choice<Number, Seq<c::OpenParen, Expression, c::CloseParen>> {};
struct Term : Seq<Factor, ZeroOrMore<FactorOp, Factor>> {};
struct Expression : Seq<Term, ZeroOrMore<TermOp, Term>> {};
struct Grammar : Seq<Expression, Must<EndOfFile>> {};

template<class Parser>
using MyAction = cst::Action<Parser, cst::DiscardRedundant<false>>;

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    for (auto str : opts.extra())
    {
	peg::cst::Tree cst;
	auto r = parse<Grammar, MyAction>(str, cst);
	cout << "match: " << r.match() << endl;

	assert(cst.nodes.size() == 1);
	assert(cst.nodes.top()->children().size() <= 1);

	if (cst.nodes.top()->children().size() == 1)
	{
	    auto root = cst.nodes.top()->move_child(0);
	    cout << root << endl;
	    
	    // cst::transform::apply<ExprPlus, cst::transform::Infix>(root);
	    // cst::transform::apply<ExprMinus, cst::transform::Infix>(root);
	    // cst::transform::apply<Grammar, cst::transform::ReplaceWithFirstChild>(root);
	    // cout << root << endl;
	}
    }

    return 0;
}

