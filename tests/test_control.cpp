// Copyright (C) 2018 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/control.h"
#include "peg/character.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

using namespace peg;

struct TrackState
{
    bool start_called = false;
    bool success_called = false;
    bool failure_called = false;
};

template<typename Parser>
struct TrackControl
{
    // template<template<typename> typename... Actions, typename... States>
    // static Input match(Input input, States&... states)
    // {
    // 	// return input;
    // 	return Machine<Parser,TrackControl,Actions...>::match(input, states...);
    // }
    
    static void start(const Input& input, TrackState& state)
    { state.start_called = true; }

    static void success(const Input& input, TrackState& state)
    { state.success_called = true; }

    static void failure(const Input& input, TrackState& state)
    { state.failure_called = true; }
};

TEST(Peg, NormalControl)
{
    using Parser = peg::Character<'a'>;
    auto r = parse<Parser,NormalControl>("a");
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, TrackControl)
{
    using Parser = peg::Character<'a'>;
    TrackState state{};
    auto s = "a"s;
    auto r = parse<Parser,TrackControl>(s, state);
    EXPECT_TRUE(state.start_called);
    EXPECT_TRUE(state.success_called);
    EXPECT_FALSE(state.failure_called);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



