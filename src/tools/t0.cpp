// Copyright (C) 2019 by Mark Melton
//

#include <typeindex>
#include <stack>
#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "core/mp/mp.h"
#include "core/string/replace.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"
#include "peg/cst/action.h"

using namespace peg;

template<class Base, class Derived>
struct Cloneable : public Base
{
    using Self = Cloneable<Base, Derived>;
    virtual ~Cloneable() = default;
    
    std::unique_ptr<Derived> clone() const
    {
	auto ptr = static_cast<const Derived*>(this);
	auto new_ptr = std::make_unique<Derived>();
	
    }
};

struct Node
{
    using Self = Node;
    using Ptr = std::unique_ptr<Self>;
    using Children = std::vector<Ptr>;
    string_view content;
    Children children;

    Ptr deep_copy() const
    {
	auto cloned_node = std::unique_ptr<Self>();
	cloned_node->content = content;
	for (auto& child : children)
	    cloned_node->children.emplace_back(child->deep_copy());
	return cloned_node;
    }
};

struct Expr;
struct Atom : Range<'a','z'> {};
struct ExprPlus : Seq<Expr, c::Plus, Atom> {};
struct ExprMinus : Seq<Expr, c::Minus, Atom> {};
struct Expr : LeftRecursion<Or<ExprPlus, ExprMinus, Atom>> {};

struct ExprPlusNode {};
struct ExprMinusNode {};

template<class Rule>
struct RuleToNode
{ using NodeType = Node; };

template<> struct RuleToNode<ExprPlus> { using NodeType = ExprPlusNode; };
template<> struct RuleToNode<ExprMinus> { using NodeType = ExprMinusNode; };

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

