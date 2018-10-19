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

struct Ignored : Optional<WhiteSpace> { };
struct Number : Sequence<Ignored,Optional<Choice<Plus,Minus>>,OneOrMore<DecimalDigit>> { };
struct Infix : Choice<Minus,Plus,Times,Divide> { };
struct Atomic : Number { };
struct Expression : Sequence<List<Atomic,Infix,Ignored>,Eof> { };

using TreeState = std::vector<std::string>;

template<typename T>
constexpr bool is_tree_node = std::is_same<T,Atomic>::value
    || std::is_same<T,Infix>::value
    || std::is_same<T,Expression>::value
    ;

template<typename T>
struct Selector
{ constexpr static bool value = is_tree_node<T>; };

template<typename Parser>
struct TreeControl
{
    static void start(const Input& input, TreeState& state)
    {
	if constexpr (not Selector<Parser>::value) return;
	state.push_back(std::string(get_name<Parser>()));
    }

    static void success(const Input& input, TreeState& state)
    {
	if constexpr (not Selector<Parser>::value) return;
    }

    static void failure(const Input& input, TreeState& state)
    {
	if constexpr (not Selector<Parser>::value) return;
	state.pop_back();
    }
};

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

    for (const auto& s : state)
	cout << s << endl;
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



