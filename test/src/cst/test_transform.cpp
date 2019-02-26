// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/cst/transform.h"

using namespace peg::cst;

struct MyNode0 : ProtoNode<MyNode0> { using Proto::Proto; };
struct MyNode1 : ProtoNode<MyNode1> { using Proto::Proto; };
    
TEST(PegCst, InfixTransform)
{
    auto n0 = Node::make<MyNode1>("expr");
    n0->emplace_child(Node::make<MyNode0>("lhs"));
    n0->emplace_child(Node::make<MyNode0>("infix"));
    n0->emplace_child(Node::make<MyNode0>("rhs"));
    
    auto n1 = Node::make<MyNode0>("infix");
    n1->emplace_child(Node::make<MyNode0>("lhs"));
    n1->emplace_child(Node::make<MyNode0>("rhs"));
    
    transform::apply<MyNode1, transform::Infix>(n0);
    EXPECT_EQ(n0, n1);
}

TEST(PegCst, ReplacWithFirstChildTransform)
{
    auto n0 = Node::make<MyNode1>("grammar");
    n0->emplace_child(Node::make<MyNode0>("expr"));
    n0->emplace_child(Node::make<MyNode0>("eof"));
    
    auto n1 = Node::make<MyNode0>("expr");
    
    transform::apply<MyNode1, transform::ReplaceWithFirstChild>(n0);
    EXPECT_EQ(n0, n1);
}

TEST(PegCst, LiftGrandChildrenTransform)
{
    auto c0 = Node::make<MyNode0>("useless");
    c0->emplace_child(Node::make<MyNode0>("lhs"));
    c0->emplace_child(Node::make<MyNode0>("infix"));
    c0->emplace_child(Node::make<MyNode0>("rhs"));
    
    auto n0 = Node::make<MyNode1>("expr");
    n0->emplace_child(std::move(c0));
    
    auto n1 = Node::make<MyNode1>("expr");
    n1->emplace_child(Node::make<MyNode0>("lhs"));
    n1->emplace_child(Node::make<MyNode0>("infix"));
    n1->emplace_child(Node::make<MyNode0>("rhs"));
    
    transform::apply<MyNode1, transform::LiftGrandChildren>(n0);
    EXPECT_EQ(n0, n1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



