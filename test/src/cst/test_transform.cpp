// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/cst/transform.h"

using namespace peg::cst;

struct MyNode0 : Prototype<DefaultNode, MyNode0> { using Proto::Proto; };
struct MyNode1 : Prototype<DefaultNode, MyNode1> { using Proto::Proto; };
    
TEST(PegCst, InfixTransform)
{
    auto n0 = DefaultNode::make<MyNode1>("expr");
    n0->emplace_child(DefaultNode::make<MyNode0>("lhs"));
    n0->emplace_child(DefaultNode::make<MyNode0>("infix"));
    n0->emplace_child(DefaultNode::make<MyNode0>("rhs"));
    
    auto n1 = DefaultNode::make<MyNode0>("infix");
    n1->emplace_child(DefaultNode::make<MyNode0>("lhs"));
    n1->emplace_child(DefaultNode::make<MyNode0>("rhs"));
    
    transform::apply<transform::Infix, MyNode1>(n0);
    EXPECT_EQ(n0, n1);
}

TEST(PegCst, ReplacWithFirstChildTransform)
{
    auto n0 = DefaultNode::make<MyNode1>("grammar");
    n0->emplace_child(DefaultNode::make<MyNode0>("expr"));
    n0->emplace_child(DefaultNode::make<MyNode0>("eof"));
    
    auto n1 = DefaultNode::make<MyNode0>("expr");
    
    transform::apply<transform::ReplaceWithFirstChild, MyNode1>(n0);
    EXPECT_EQ(n0, n1);
}

TEST(PegCst, LiftGrandChildrenTransform)
{
    auto c0 = DefaultNode::make<MyNode0>("useless");
    c0->emplace_child(DefaultNode::make<MyNode0>("lhs"));
    c0->emplace_child(DefaultNode::make<MyNode0>("infix"));
    c0->emplace_child(DefaultNode::make<MyNode0>("rhs"));
    
    auto n0 = DefaultNode::make<MyNode1>("expr");
    n0->emplace_child(std::move(c0));
    
    auto n1 = DefaultNode::make<MyNode1>("expr");
    n1->emplace_child(DefaultNode::make<MyNode0>("lhs"));
    n1->emplace_child(DefaultNode::make<MyNode0>("infix"));
    n1->emplace_child(DefaultNode::make<MyNode0>("rhs"));
    
    transform::apply<transform::LiftGrandChildren, MyNode1>(n0);
    EXPECT_EQ(n0, n1);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



