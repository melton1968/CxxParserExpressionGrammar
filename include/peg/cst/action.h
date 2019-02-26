// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include <typeindex>
#include "peg/action.h"
#include "peg/cst/tree.h"
#include "peg/cst/transform.h"

namespace peg::cst
{

template<bool V>
struct DiscardChildren
{ static constexpr bool DiscardChildrenValue = V; };

template<class T>
struct GetDiscardChildren
{ static constexpr bool value = false; };

template<class T>
requires ((T::DiscardChildrenValue == true))
struct GetDiscardChildren<T>
{ static constexpr bool value = true; };

template<bool V>
struct DiscardRedundant
{ static constexpr bool DiscardRedundantValue = V; };

template<class T>
struct GetDiscardRedundant
{ static constexpr bool value = true; };

template<class T>
requires ((T::DiscardRedundantValue == false))
struct GetDiscardRedundant<T>
{ static constexpr bool value = false; };

template<class Parser, class... Parameters>
struct BaseAction : NullAction<Parser>
    , Parameters...
{
    using Self = BaseAction<Parser, Parameters...>;
    
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

	if constexpr (GetDiscardChildren<Parser>::value) n->children().clear();
	else if (GetDiscardRedundant<Self>::value) n = discard_redundant_nodes(std::move(n));
	
	n->set_content(input.match());
	pt.nodes.top()->emplace_child(std::move(n));

	EnsureGT(pt.nodes.size(), 0);
    }

    static Node::Ptr discard_redundant_nodes(Node::Ptr n)
    {
	while (n->children().size() == 1 and Node::TypeId == typeid(*n))
	    n = n->move_child(0);
	
	while (n->children().size() == 1 and Node::TypeId == typeid(*(n->child(0))))
	    transform::LiftGrandChildren::apply(n);
		
	return std::move(n);
    }
	
};

template<class Parser, class... Parameters>
struct Action : BaseAction<Parser, Parameters...>
{};

template<class Parser, class... Parameters>
requires ((Parser::IsLeftRecursionParser == true))
struct Action<Parser, Parameters...> : BaseAction<Parser, Parameters...>
{
    using Self = Action<Parser, Parameters...>;
    using Base = BaseAction<Parser, Parameters...>;
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
	ExpectEQ(pt.nodes.top()->children().size(), 1);
	
	auto n = pt.nodes.top()->move_child(0);
	pt.nodes.top()->children().pop_back();

	if constexpr (GetDiscardRedundant<Self>::value)
			 n = Base::discard_redundant_nodes(std::move(n));

	if (nodes().find(begin) == nodes().end())
	    nodes().insert_or_assign(begin, std::move(n));
	else
	{
	    transform::ReplaceMatchingSubtree::apply(n, nodes()[begin]);
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
	    pt.nodes.top()->emplace_child(std::move(new_node));
	}
    }

    static void recursion_end(const char *begin, const Input& input, Tree& pt)
    {
	if (nodes().find(begin) != nodes().end())
	{
	    ExpectGT(pt.nodes.size(), 0);
	    
	    auto n = std::move(nodes()[begin]);
	    pt.nodes.top()->children().clear();
	    pt.nodes.top()->emplace_child(std::move(n));
	    
	}
	nodes().clear();
    }
};

}; // end ns peg::cst
