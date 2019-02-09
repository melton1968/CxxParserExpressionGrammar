// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/character.h"
#include "peg/parse.h"

TEST(Peg, CharacterSuccess)
{
    using Parser = peg::Character<'a'>;
    string str = "a";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, CharacterFailure)
{
    using Parser = peg::Character<'a'>;
    string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NotCharacterSuccess)
{
    using Parser = peg::NotCharacter<'a'>;
    string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "b");
}

TEST(Peg, NotCharacterFailure)
{
    using Parser = peg::NotCharacter<'a'>;
    string str = "a";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, CharactersSuccess)
{
    using Parser = peg::Characters<'a','b'>;
    string str = "ab";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "ab");
}

TEST(Peg, CharactersFailure)
{
    using Parser = peg::Characters<'a','b'>;
    string str = "ac";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

struct tmp { constexpr static const char s[] = "abcdef"; };
    
TEST(Peg, StringSuccess)
{
    using Parser = peg::String<tmp::s>;
    string str = "abcdef";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "abcdef");
}

TEST(Peg, StringFailure)
{
    using Parser = peg::String<tmp::s>;
    string str = "abCdef";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



