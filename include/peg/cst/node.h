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
    string_view content;
    Children children;

    Ptr clone() const
    {
	auto n = std::make_unique<Self>();
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

}; // end ns peg::cst
