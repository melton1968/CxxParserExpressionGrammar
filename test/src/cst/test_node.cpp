// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/cst/node.h"

TEST(PegCst, Node)
{
    struct MyNode : peg::cst::ProtoNode<MyNode> { using Proto::Proto; };
    auto n = std::make_unique<peg::cst::Node>("parent");
    auto c0 = std::make_unique<peg::cst::Node>("child0");
    auto c1 = std::make_unique<MyNode>("child1");
    
    n->emplace_child(std::move(c0));
    n->emplace_child(std::move(c1));

    EXPECT_EQ(n->content(), "parent");
    EXPECT_EQ(n->child(0)->content(), "child0");
    EXPECT_EQ(n->child(1)->content(), "child1");

    EXPECT_EQ(typeid(peg::cst::Node), n->tid());
    EXPECT_EQ(typeid(peg::cst::Node), n->child(0)->tid());
    EXPECT_EQ(typeid(MyNode), n->child(1)->tid());
    
    auto copy = n->clone();
    EXPECT_TRUE(n == copy);
    
    EXPECT_EQ(copy->content(), "parent");
    EXPECT_EQ(copy->child(0)->content(), "child0");
    EXPECT_EQ(copy->child(1)->content(), "child1");

    EXPECT_EQ(typeid(peg::cst::Node), copy->tid());
    EXPECT_EQ(typeid(peg::cst::Node), copy->child(0)->tid());
    EXPECT_EQ(typeid(MyNode), copy->child(1)->tid());
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



