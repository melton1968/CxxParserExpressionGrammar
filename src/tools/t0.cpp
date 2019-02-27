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

struct ConcreteNode : cst::Node<ConcreteNode>
{ 
    ConcreteNode() = default;
    virtual ~ConcreteNode() = default;
    virtual index_t eval() const { assert(false); return 0; }
};

template<class Derived>
struct ProtoNode : cst::Prototype<ConcreteNode, Derived>
{ };

struct Number : ProtoNode<Number>, NoSkip<OneOrMore<Range<'0','9'>>>
{ virtual index_t eval() const override { return core::lexical_cast<int>(content()); } };

struct Mul : ProtoNode<Mul>, c::Multiply
{ virtual index_t eval() const override { return child(0)->eval() * child(1)->eval(); } };

struct Div : ProtoNode<Div>, c::Divide
{ virtual index_t eval() const override { return child(0)->eval() / child(1)->eval(); } };

struct Add : ProtoNode<Add>, c::Plus
{ virtual index_t eval() const override { return child(0)->eval() + child(1)->eval(); } };

struct Sub : ProtoNode<Sub>, c::Minus
{ virtual index_t eval() const override { return child(0)->eval() - child(1)->eval(); } };

struct FactorOp : Choice<Mul, Div> {};
struct TermOp : Choice<Add, Sub> {};

struct Expression;
struct ExpressionGrouping : ProtoNode<ExpressionGrouping>
			  , Seq<c::OpenParen, Expression, c::CloseParen> {};

struct Factor : Choice<Number, ExpressionGrouping>
	      , ProtoNode<Factor>
{};

struct Term : LeftRecursion<Choice<Seq<Term, FactorOp, Factor>,	Factor>>
	    , ProtoNode<Term>
{};

struct Expression : LeftRecursion<Choice<Seq<Expression, TermOp, Term>, Term>>
		  , ProtoNode<Expression>
{};

struct Grammar : SkipWhiteSpace<Seq<Expression, Must<EndOfFile>>>
	       , ProtoNode<Grammar> {};

template<class Parser>
using MyAction = cst::Action<Parser, ConcreteNode, cst::DiscardRedundant<true>>;

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    for (auto str : opts.extra())
    {
        peg::cst::Tree<ConcreteNode> cst;
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
	    cout << root->eval() << endl;
	}
    }

    return 0;
}

