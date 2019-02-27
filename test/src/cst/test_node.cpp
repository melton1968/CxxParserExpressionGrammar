// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/cst/node.h"

using namespace peg::cst;

TEST(PegCst, Node)
{
    struct MyNode : Prototype<DefaultNode, MyNode> { using Proto::Proto; };
    auto n = DefaultNode::make<DefaultNode>("parent");
    auto c0 = DefaultNode::make<DefaultNode>("child0");
    auto c1 = DefaultNode::make<MyNode>("child1");
    
    n->emplace_child(std::move(c0));
    n->emplace_child(std::move(c1));

    EXPECT_EQ(n->content(), "parent");
    EXPECT_EQ(n->child(0)->content(), "child0");
    EXPECT_EQ(n->child(1)->content(), "child1");

    EXPECT_EQ(typeid(DefaultNode), n->tid());
    EXPECT_EQ(typeid(DefaultNode), n->child(0)->tid());
    EXPECT_EQ(typeid(MyNode), n->child(1)->tid());
    
    auto copy = n->clone();
    EXPECT_TRUE(n == copy);
    
    EXPECT_EQ(copy->content(), "parent");
    EXPECT_EQ(copy->child(0)->content(), "child0");
    EXPECT_EQ(copy->child(1)->content(), "child1");

    EXPECT_EQ(typeid(DefaultNode), copy->tid());
    EXPECT_EQ(typeid(DefaultNode), copy->child(0)->tid());
    EXPECT_EQ(typeid(MyNode), copy->child(1)->tid());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



