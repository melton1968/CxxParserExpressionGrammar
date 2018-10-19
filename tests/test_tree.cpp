// Copyright (C) 2018 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/control.h"
#include "peg/character.h"
#include "peg/repetition.h"
#include "peg/whitespace.h"
#include "peg/atomic.h"

using ::testing::StaticAssertTypeEq;

using namespace peg;

struct TreeState
{ };

template<typename Parser>
struct TreeControl
{
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
using Number = Sequence<Ignored,Optional<Choice<Plus,Minus>>,OneOrMore<DecimalDigit>>;
using Infix = Choice<Minus,Plus,Times,Divide>;
using Atomic = Number;
using Expression = Sequence<List<Atomic,Infix,Ignored>,Eof>;


TEST(Peg, TreeControl0)
{
    using Parser = Expression;
    TreeState state;
    auto s = "1"s;
    auto r = Machine<Parser,TreeControl>::match(s, state);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "1");
}

TEST(Peg, TreeControl1)
{
    using Parser = Expression;
    TreeState state;
    auto s = "1 + 1"s;
    auto r = Machine<Parser,TreeControl>::match(s, state);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "1 + 1");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



