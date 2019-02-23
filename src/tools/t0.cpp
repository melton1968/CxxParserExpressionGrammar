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

using namespace peg;

namespace peg
{

struct Node
{
    using Ptr = std::unique_ptr<Node>;
    using Children = std::vector<Ptr>;
    string_view content;
    Children children;

    Ptr clone() const
    {
	auto n = std::make_unique<Node>();
	n->content = content;
	for (auto& c : children)
	{
	    auto new_child = c->clone();
	    n->children.emplace_back(std::move(new_child));
	}
	return n;
    }
};

void print(Node::Ptr& node, size_t level = 0)
{
    cout << "   ";
    for (size_t i = 0; i < level; ++i)
	cout << "|   ";
    cout << "'" << node->content << "'" << endl;
    for (auto& n : node->children)
	print(n, level+1);
}

struct Tree
{
    std::stack<Node::Ptr> nodes;
    size_t tree_level{0};
    size_t parse_level{0};
};

template<class Parser>
struct ParseTreeAction : ::NullAction<Parser>
{
    static void start(const Input& input, Tree& pt)
    { pt.nodes.emplace(std::make_unique<Node>()); }
    
    static void failure(const Input& input, Tree& pt)
    { pt.nodes.pop(); }
    
    static void success(const Input& input, Tree& pt)
    {
	auto n = std::move(pt.nodes.top());
	pt.nodes.pop();
	n->content = input.match();
	pt.nodes.top()->children.emplace_back(std::move(n));
    }
};

void replace_matching_nodes(Node::Ptr& node, Node::Ptr& new_node)
{
    if (node->content == new_node->content)
    {
	node = new_node->clone();
	return;
    }
    for (auto& n : node->children)
	replace_matching_nodes(n, new_node);
}

template<class T>
requires (T::IsLeftRecursionParser == true)
struct ParseTreeAction<T> : ParseTreeAction<void>
{
    using Base = ParseTreeAction<void>;
    using Nodes = std::map<const char*, Node::Ptr>;

    static Nodes& nodes()
    {
	static Nodes static_nodes;
	return static_nodes;
    }
    
    static void start(const Input& input, Tree& pt)
    { Base::start(input, pt); }
    
    static void failure(const Input& input, Tree& pt)
    { Base::failure(input, pt); }
    
    static void success(const Input& input, Tree& pt)
    { Base::success(input, pt); }

    static void recursion_begin(const Input& input, Tree& pt)
    { }

    static void recursion_success(const char *begin, const Input& input, Tree& pt)
    {
	auto base = std::move(pt.nodes.top()->children.back());
	pt.nodes.top()->children.pop_back();

	auto n = std::move(base->children.back());

	if (nodes().find(begin) == nodes().end())
	    nodes().insert_or_assign(begin, std::move(n));
	else
	{
	    auto previous_node = std::move(nodes()[begin]);
	    replace_matching_nodes(n, previous_node);
	    nodes()[begin] = std::move(n);
	}
    }

    static void recursion_failure(const char *begin, const Input& input, Tree& pt)
    { }

    static void recursion_matched(const char *begin, const Input& input, Tree& pt)
    {
	if (input.status())
	{
	    auto& n = nodes()[begin];
	    assert(n);
	    auto new_node = n->clone();
	    pt.nodes.top()->children.emplace_back(std::move(new_node));
	}
    }

    static void recursion_end(const char *begin, const Input& input, Tree& pt)
    {
	pt.nodes.pop();
	pt.nodes.emplace(std::move(nodes()[begin]));
	nodes().clear();
    }
};

}; // end ns peg

// struct Digit : Range<'0','9'> {};
// struct Atom : Range<'a','z'> {};
// struct Expr : Or<OneOrMore<Atom>, OneOrMore<Digit>> {};

struct Atom : Range<'a','z'> {};
struct Expr : LeftRecursion<
    Or<
	Seq<Expr, c::Plus, Atom>,
	Seq<Expr, c::Minus, Atom>,
	Atom
	>
    >
{};


int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    peg::expr::apply_rt<peg::expr::printer, std::tuple<Expr>>::apply(cout);
    cout << endl;

    for (auto str : opts.extra())
    {
	peg::Tree parse_tree;
	parse_tree.nodes.push(std::make_unique<peg::Node>());
	auto r = parse<Expr, ParseTreeAction>(str, parse_tree);
	cout << "match: " << r.match() << endl;

	assert(parse_tree.nodes.size() == 1);
	assert(parse_tree.nodes.top()->children.size() == 1);
	
	auto& root = parse_tree.nodes.top()->children.back();
	print(root);
    }

    return 0;
}

