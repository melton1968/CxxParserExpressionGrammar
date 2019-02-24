// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "core/common.h"

namespace peg::cst
{

struct Node
{
    using Self = Node;
    using Ptr = std::unique_ptr<Self>;
    using Children = std::vector<Ptr>;
    
    virtual ~Node() = default;
    virtual Ptr make_unique() const { return std::make_unique<Self>(); }
    
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
};

namespace detail
{

template<class Base, class Derived>
struct Prototype : public Base
{
    static constexpr bool IsNode = true;
    virtual ~Prototype() = default;
    virtual std::unique_ptr<Base> make_unique() const
    { return std::make_unique<Derived>(); }
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
    cout << "\t\t\t" << core::type_name(*node);
    cout << endl;

    for (auto& n : node->children)
	print(n, level+1);
}

}; // end ns peg::cst
