// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/common.h"

namespace peg::cst
{

struct Node
{
    static constexpr bool IsNode = true;
    using Self = Node;
    using Ptr = std::unique_ptr<Self>;
    using Children = std::vector<Ptr>;
    
    virtual ~Node() = default;
    virtual Ptr make_unique() const { return std::make_unique<Self>(); }
    virtual string type_name() const { return core::type_name<Self>(); }
    
    string_view content;
    Children children;

    Ptr clone() const
    {
	auto n = make_unique();
	n->content = content;
	for (auto& c : children)
	{
	    auto new_child = c->clone();
	    n->children.emplace_back(std::move(new_child));
	}
	return n;
    }

    void replace_children_with_grandchildren()
    {
	Children new_children;
	for (auto& child : children)
	    for (auto& grandchild : child->children)
		new_children.emplace_back(std::move(grandchild));
	std::swap(children, new_children);
    }

    static const std::type_index TypeId;
};

const std::type_index Node::TypeId = typeid(Node);

namespace detail
{

template<class Base, class Derived>
struct Prototype : public Base
{
    virtual ~Prototype() = default;
    
    virtual std::unique_ptr<Base> make_unique() const override
    { return std::make_unique<Derived>(); }
    
    virtual string type_name() const override { return core::type_name<Derived>(); }
};

}; // end ns detail

template<class Derived>
using ProtoNode = detail::Prototype<Node, Derived>;

template<class Node>
concept bool IsNode = Node::IsNode == true;

void print(Node::Ptr& node, size_t level = 0)
{
    cout << "   ";
    for (size_t i = 0; i < level; ++i)
	cout << "|   ";
    cout << "'" << node->content << "'";
    cout << "    " << core::type_name(*node);
    cout << endl;

    for (auto& n : node->children)
	print(n, level+1);
}

}; // end ns peg::cst
