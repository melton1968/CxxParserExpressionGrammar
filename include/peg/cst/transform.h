// Copyright (C) 2019, 2021 by Mark Melton
//

#pragma once
#include "peg/cst/node.h"

namespace peg::cst::transform
{

struct Infix
{
    static void apply(auto& n)
    {
	ExpectEQ(n->children().size(), 3);

	auto new_node = n->move_child(1);
	new_node->emplace_child(n->move_child(0));
	new_node->emplace_child(n->move_child(2));
	n = std::move(new_node);
	
	EnsureEQ(n->children().size(), 2);
    }
};

struct MaybeInfix
{
    static void apply(auto& n)
    {
	if (n->children().size() == 3)
	    Infix::apply(n);
    }
};

struct ReplaceWithFirstChild
{
    static void apply(auto& n)
    {
	ExpectGT(n->children().size(), 0);
	n = n->move_child(0);
    }
};

struct ReplaceWithSecondChild
{
    static void apply(auto& n)
    {
	ExpectGT(n->children().size(), 1);
	n = n->move_child(1);
    }
};

struct MaybeReplaceWithOnlyChild
{
    static void apply(auto& n)
    {
	if (n->children().size() == 1)
	    n = n->move_child(0);
    }
};

struct LiftGrandChildren
{
    static void apply(auto& n)
    {
	using NodeType = std::remove_reference_t<decltype(*n)>;
	using Children = typename NodeType::Children;
	Children new_children;
	for (auto& child : n->children())
	    for (auto& grandchild : child->children())
		new_children.emplace_back(std::move(grandchild));
	std::swap(n->children(), new_children);
    }
};

struct DiscardChildren
{
    static void apply(auto& n)
    {
	n->children().clear();
    }
};

struct ReplaceMatchingSubtree
{
    static void apply(auto& n, const auto& replacement)
    {
	if (n->content() == replacement->content())
	{
	    n = replacement->clone();
	    return;
	}
	for (auto& child : n->children())
	    apply(child, replacement);
    }
};

// template<class T, class... Us>
// void apply(auto& n)
// {
//     for (auto& child : n->children())
// 	apply<T,Us...>(child);

//     if ((typeid(Us) == typeid(*n) or ...))
// 	    T::apply(n);
// }

}; // end ns peg::cst::transform

