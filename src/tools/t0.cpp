// Copyright (C) 2019 by Mark Melton
//

#include "core/tool.h"
#include "core/demangle.h"
#include "core/string/lexical_cast.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"
#include "peg/cst/action.h"
#include "peg/cst/transform.h"

using namespace peg;

struct Number : cst::ProtoNode<Number>
	      , NoSkip<OneOrMore<Range<'0','9'>>>
{ };

struct Mul : cst::ProtoNode<Mul>
	   , c::Multiply
{ };

struct Div : cst::ProtoNode<Div>
		, c::Divide
{ };

struct Add : cst::ProtoNode<Add>
	   , c::Plus
{ };

struct Sub : cst::ProtoNode<Sub>
	   , c::Minus
{ };

struct FactorOp : Choice<Mul, Div> {};
struct TermOp : Choice<Add, Sub> {};

struct Expression;
struct ExpressionGrouping : cst::ProtoNode<ExpressionGrouping>
			  , Seq<c::OpenParen, Expression, c::CloseParen> {};

struct Factor : Choice<Number, ExpressionGrouping>
	      , cst::ProtoNode<Factor>
{};

struct Term : LeftRecursion<Choice<Seq<Term, FactorOp, Factor>,	Factor>>
	    , cst::ProtoNode<Term>
{};

struct Expression : LeftRecursion<Choice<Seq<Expression, TermOp, Term>, Term>>
		  , cst::ProtoNode<Expression>
{};

struct Grammar : SkipWhiteSpace<Seq<Expression, Must<EndOfFile>>>
	       , cst::ProtoNode<Grammar> {};

// struct Expression;
// struct Factor : Choice<Number, Seq<c::OpenParen, Expression, c::CloseParen>> {};
// struct Term : Seq<Factor, ZeroOrMore<FactorOp, Factor>> {};
// struct Expression : Seq<Term, ZeroOrMore<TermOp, Term>> {};
// struct Grammar : SkipWhiteSpace<Seq<Expression, Must<EndOfFile>>> {};

template<class Parser>
using MyAction = cst::Action<Parser, cst::Node, cst::DiscardRedundant<true>>;

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    for (auto str : opts.extra())
    {
        peg::cst::Tree<cst::Node> cst;
	auto r = parse<Grammar, MyAction>(str, cst);
	cout << "match: " << r.match() << endl;

	assert(cst.nodes.size() == 1);
	assert(cst.nodes.top()->children().size() <= 1);

	if (cst.nodes.top()->children().size() == 1)
	{
	    auto root = cst.nodes.top()->move_child(0);
	    cout << root << endl;

	    cst::transform::apply<Factor, cst::transform::MaybeInfix>(root);
	    cst::transform::apply<Term, cst::transform::MaybeInfix>(root);
	    cst::transform::apply<Expression, cst::transform::MaybeInfix>(root);
	    cst::transform::apply<ExpressionGrouping,cst::transform::ReplaceWithSecondChild>(root);
	    cst::transform::apply<Factor, cst::transform::MaybeReplaceWithOnlyChild>(root);
	    cst::transform::apply<Term, cst::transform::MaybeReplaceWithOnlyChild>(root);
	    cst::transform::apply<Expression, cst::transform::MaybeReplaceWithOnlyChild>(root);
	    cst::transform::apply<Grammar, cst::transform::ReplaceWithFirstChild>(root);
	    cout << root << endl;
	}
    }

    return 0;
}

