// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/cst/node.h"

namespace peg::cst::transform
{

struct Infix
{
    static void apply(Node::Ptr& n)
    {
	ExpectEQ(n->children.size(), 3);

	auto new_node = std::move(n->children[1]);
	new_node->children.emplace_back(std::move(n->children[0]));
	new_node->children.emplace_back(std::move(n->children[2]));
	n = std::move(new_node);
	
	EnsureEQ(n->children.size(), 2);
    }
};

struct ReplaceWithFirstChild
{
    static void apply(Node::Ptr& n)
    {
	ExpectGT(n->children.size(), 0);
	n = std::move(n->children.front());
    }
};

struct LiftGrandChildren
{
    static void apply(Node::Ptr& n)
    {
	Node::Children new_children;
	for (auto& child : n->children)
	    for (auto& grandchild : child->children)
		new_children.emplace_back(std::move(grandchild));
	std::swap(n->children, new_children);
    }
};

template<class T, class U>
void apply(Node::Ptr& n)
{
    for (auto& child : n->children)
	apply<T,U>(child);
    
    if (typeid(T) == typeid(*n))
	U::apply(n);
}

}; // end ns peg::cst::transform

