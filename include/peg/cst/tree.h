// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include <stack>
#include "peg/cst/node.h"

namespace peg::cst
{

struct Tree
{
    Tree()
    {
	nodes.push(std::make_unique<Node>());
    }
    
    std::stack<Node::Ptr> nodes;
    size_t tree_level{0};
    size_t parse_level{0};
};

}; // end ns peg::cst
