// Copyright (C) 2019 by Mark Melton
//

#pragma once
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
struct Action : NullAction<Parser>
{
    static void start(const Input& input, Tree& pt)
    {
	if constexpr (IsNode<Parser>) pt.nodes.emplace(std::make_unique<Parser>());
	else pt.nodes.emplace(std::make_unique<Node>());
    }
    
    static void failure(const Input& input, Tree& pt)
    {
	pt.nodes.pop();
    }
    
    static void success(const Input& input, Tree& pt)
    {
	auto n = std::move(pt.nodes.top());
	pt.nodes.pop();

	n->content = input.match();
	if constexpr (DiscardChildrenValue<Parser>::value) n->children.clear();

	assert(pt.nodes.size() > 0);
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
requires ((T::IsLeftRecursionParser == true))
struct Action<T> : Action<void>
{
    using Base = Action<void>;
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
	if (nodes().find(begin) != nodes().end())
	{
	    pt.nodes.pop();
	    pt.nodes.emplace(std::move(nodes()[begin]));
	}
	nodes().clear();
    }
};

}; // end ns peg::cst
