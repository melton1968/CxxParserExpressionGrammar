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
};

void print(Node::Ptr& node, size_t level = 0)
{
    cout << " ";
    for (size_t i = 1; i < level; ++i)
	cout << "   |";
    cout << "   ";
    cout << node->content << endl;
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
    {
	++pt.parse_level;
    }
    
    static void failure(const Input& input, Tree& pt)
    {
	--pt.parse_level;
    }
    
    static void success(const Input& input, Tree& pt)
    {
	while (++pt.tree_level <= pt.parse_level)
	    pt.nodes.push(std::make_unique<Node>());
	
	--pt.parse_level;
	--pt.tree_level;
	--pt.tree_level;

	assert(pt.nodes.size() > 1);
	auto n = std::move(pt.nodes.top());
	pt.nodes.pop();
	n->content = input.match();
	pt.nodes.top()->children.emplace_back(std::move(n));
    }
};

template<class T>
requires (T::IsLeftRecursionParser == true)
struct ParseTreeAction<T> : ParseTreeAction<void>
{
    using Base = ParseTreeAction<void>;
    using Nodes = std::stack<Node::Ptr>;

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

    static void begin_recursion(const Input& input, Tree& pt)
    { }

    static void success_recursion(const Input& input, Tree& pt)
    {
	cout << pt.nodes.top()->children.size() << endl;

	auto n = std::move(pt.nodes.top()->children.back());
	pt.nodes.top()->children.pop_back();
	nodes().emplace(std::move(n));
    }

    static void failure_recursion(const Input& input, Tree& pt)
    {
	pt.nodes.top()->children.pop_back();
    }

    static void end_recursion(const Input& input, Tree& pt)
    {
	cout << pt.nodes.top()->children.size() << endl;
	while (nodes().size() > 0)
	{
	    auto& n = nodes().top();
	    pt.nodes.top()->children.emplace_back(std::move(n));
	    nodes().pop();
	}
    }
};

}; // end ns peg

struct Expr : LeftRecursion<
    Or<
	Seq<Expr, c::Plus, c::a>,
	c::a
	>
    >
{};

struct Expr1 : Or<
    Seq<LeftRecursion<Expr1>, c::Plus, c::a>,
    c::a
    >
{};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    peg::expr::apply_rt<peg::expr::printer, std::tuple<Expr>>::apply(cout);
    cout << endl;

    string str = "a+a+a+a";
    peg::Tree parse_tree;
    parse_tree.nodes.push(std::make_unique<peg::Node>());
    auto r = parse<Expr, ParseTreeAction>(str, parse_tree);
    cout << "match: " << r.match() << endl;

    auto root = std::move(parse_tree.nodes.top());
    parse_tree.nodes.pop();
    assert(parse_tree.nodes.empty());

    print(root);

    return 0;
}

