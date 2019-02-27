// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/peg.h"
#include "peg/cst/action.h"
#include "peg/cst/transform.h"

using namespace peg;

namespace calculator
{

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

struct Term;
struct Expression;

struct ExpressionGrouping : ProtoNode<ExpressionGrouping>
			  , Seq<c::OpenParen, Expression, c::CloseParen> {};

struct Factor : Choice<Number, ExpressionGrouping>, ProtoNode<Factor> {};
struct FactorExpr : Seq<Term, FactorOp, Factor>, ProtoNode<FactorExpr> {};
struct Term : LeftRecursion<Choice<FactorExpr, Factor>>, ProtoNode<Term> {};
struct TermExpr : Seq<Expression, TermOp, Term>, ProtoNode<TermExpr> {};
struct Expression : LeftRecursion<Choice<TermExpr, Term>>, ProtoNode<Expression> {};
struct Grammar : SkipWhiteSpace<Seq<Expression, Must<EndOfFile>>>, ProtoNode<Grammar> {};

template<class Parser>
using Action = cst::Action<Parser, ConcreteNode, cst::DiscardRedundant<true>>;

using Tree = cst::Tree<ConcreteNode>;
namespace x = peg::cst::transform;

int parse(const string& str)
{
    Tree cst;
    auto r = peg::parse<Grammar, Action>(str, cst);

    assert(r);
    assert(cst.nodes.size() == 1);
    assert(cst.nodes.top()->children().size() <= 1);
    assert(cst.nodes.top()->children().size() == 1);
	
    auto root = cst.nodes.top()->move_child(0);
    x::apply<x::DiscardChildren, Number>(root);
    x::apply<x::Infix, FactorExpr, TermExpr>(root);
    x::apply<x::ReplaceWithSecondChild, ExpressionGrouping>(root);
    x::apply<x::MaybeReplaceWithOnlyChild, Factor, Term, Expression>(root);
    x::apply<x::ReplaceWithFirstChild, Grammar>(root);
    return root->eval();
}

}; // end ns calculator
