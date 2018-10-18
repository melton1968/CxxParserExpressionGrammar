// Copyright (C) 2018 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/control.h"
#include "peg/character.h"
#include "peg/repetition.h"
#include "peg/whitespace.h"

using ::testing::StaticAssertTypeEq;

using namespace peg;

struct TreeState
{ };

template<typename Parser>
struct TreeControl
{
    // template<template<typename> typename... Actions, typename... States>
    // static Input match(Input input, States&... states)
    // {
    // 	// return input;
    // 	return Machine<Parser,TrackControl,Actions...>::match(input, states...);
    // }
    
    static void start(const Input& input, TreeState& state)
    {
    }

    static void success(const Input& input, TreeState& state)
    {
    }

    static void failure(const Input& input, TreeState& state)
    {
    }
};

using Ignored = Optional<WhiteSpace>;
using Number = Sequence<Optional<Choice<Plus,Minus>>,OneOrMore<DecimalDigit>>;
using Infix = Choice<Minus,Plus,Times,Divide>;
using Atomic = Number;
using Expression = List<Atomic,Infix,Ignored>;


TEST(Peg, TreeControl)
{
    using Parser = Expression;
    TreeState state;
    auto s = "1"s;
    auto r = Machine<Parser,TreeControl>::match(s, state);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "1");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



