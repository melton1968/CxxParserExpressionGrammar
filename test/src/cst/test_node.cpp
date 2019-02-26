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
    
    n->children.emplace_back(std::move(c0));
    n->children.emplace_back(std::move(c1));

    EXPECT_EQ(n->content, "parent");
    EXPECT_EQ(n->children[0]->content, "child0");
    EXPECT_EQ(n->children[1]->content, "child1");

    EXPECT_EQ(typeid(peg::cst::Node), typeid(*n));
    EXPECT_EQ(typeid(peg::cst::Node), typeid(*(n->children[0])));
    EXPECT_EQ(typeid(MyNode), typeid(*(n->children[1])));
    
    auto copy = n->clone();
    EXPECT_EQ(copy->content, "parent");
    EXPECT_EQ(copy->children[0]->content, "child0");
    EXPECT_EQ(copy->children[1]->content, "child1");

    EXPECT_EQ(typeid(peg::cst::Node), typeid(*n));
    EXPECT_EQ(typeid(peg::cst::Node), typeid(*(copy->children[0])));
    EXPECT_EQ(typeid(MyNode), typeid(*(copy->children[1])));
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



