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
    
    static void start(const Input& input, Tree& pt)
    { Base::start(input, pt); }
    
    static void failure(const Input& input, Tree& pt)
    { Base::failure(input, pt); }
    
    static void success(const Input& input, Tree& pt)
    {
	cout << "left recursion success: " << input.match() << endl;
	Base::success(input, pt);
    }

    static void left_recursion_complete(const Input& input, Tree& pt)
    {
	cout << "left recursion complete: " << input.match() << endl;
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

void print(peg::Node::Ptr& node, size_t level = 0)
{
    cout << string(4 * level, ' ') << node->content << endl;
    for (auto& n : node->children)
	print(n, level+1);
}

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

