// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include <typeindex>
#include "peg/cst/tree.h"
#include "peg/action.h"

namespace peg::cst
{

template<bool V>
struct DiscardChildren
{ static constexpr bool DiscardChildrenValue = V; };

template<class T>
struct DiscardChildrenValue
{ static constexpr bool value = false; };

template<class T>
requires ((T::DiscardChildrenValue == true))
struct DiscardChildrenValue<T>
{ static constexpr bool value = true; };

template<class Parser>
struct BaseAction : NullAction<Parser>
{
    static void start(const Input& input, Tree& pt)
    {
	ExpectGT(pt.nodes.size(), 0);
	
	if constexpr (IsNode<Parser>) pt.nodes.emplace(std::make_unique<Parser>());
	else pt.nodes.emplace(std::make_unique<Node>());

	EnsureGT(pt.nodes.size(), 1);
    }
    
    static void failure(const Input& input, Tree& pt)
    {
	ExpectGT(pt.nodes.size(), 1);
	pt.nodes.pop();
	EnsureGT(pt.nodes.size(), 0);
    }
    
    static void success(const Input& input, Tree& pt)
    {
	ExpectGT(pt.nodes.size(), 1);
	
	auto n = std::move(pt.nodes.top());
	pt.nodes.pop();

	if constexpr (DiscardChildrenValue<Parser>::value) n->children.clear();
	else n = discard_redundant_nodes(std::move(n));
	
	n->content = input.match();
	pt.nodes.top()->children.emplace_back(std::move(n));

	print(pt.nodes.top());
	cout << endl;
	
	EnsureGT(pt.nodes.size(), 0);
    }

    static Node::Ptr discard_redundant_nodes(Node::Ptr n)
    {
	while (n->children.size() == 1 and Node::TypeId == typeid(*n))
	    n = std::move(n->children.back());
	return std::move(n);
    }
	
};

template<class Parser>
struct Action : BaseAction<Parser>
{};

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

template<class Parser>
requires ((Parser::IsLeftRecursionParser == true))
struct Action<Parser> : BaseAction<Parser>
{
    using Base = BaseAction<Parser>;
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
	ExpectGT(pt.nodes.size(), 0);
	ExpectEQ(pt.nodes.top()->children.size(), 1);
	
	auto n = std::move(pt.nodes.top()->children.back());
	pt.nodes.top()->children.pop_back();

	n = Base::discard_redundant_nodes(std::move(n));

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
	    ExpectTRUE(nodes().find(begin) != nodes().end());
	    ExpectGT(pt.nodes.size(), 0);

	    auto new_node = nodes()[begin]->clone();
	    pt.nodes.top()->children.emplace_back(std::move(new_node));
	}
    }

    static void recursion_end(const char *begin, const Input& input, Tree& pt)
    {
	if (nodes().find(begin) != nodes().end())
	{
	    ExpectGT(pt.nodes.size(), 0);
	    
	    auto n = std::move(nodes()[begin]);
	    pt.nodes.top()->children.clear();
	    pt.nodes.top()->children.emplace_back(std::move(n));
	    
	}
	nodes().clear();
    }
};

}; // end ns peg::cst
