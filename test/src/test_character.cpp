// Copyright (C) 2018, 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/character.h"
#include "peg/parse.h"

TEST(Peg, CharacterSuccess)
{
    using Parser = peg::Character<'a','b'>;
    string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "b");
}

TEST(Peg, CharacterFailure)
{
    using Parser = peg::Character<'a','b'>;
    string str = "c";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, CharacterCaselessSuccess)
{
    using Parser = peg::CharacterCaseless<'a','b'>;
    string str = "B";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "B");
}

TEST(Peg, CharacterCaselessFailure)
{
    using Parser = peg::CharacterCaseless<'a','b'>;
    string str = "C";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NotCharacterSuccess)
{
    using Parser = peg::NotCharacter<'a','b'>;
    string str = "c";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "c");
}

TEST(Peg, NotCharacterFailure)
{
    using Parser = peg::NotCharacter<'a','b'>;
    string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, AnyCharacterSuccess)
{
    using Parser = peg::AnyCharacter;
    string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "b");
}

TEST(Peg, StringSuccess)
{
    using Parser = peg::String<'a','b'>;
    string str = "ab";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "ab");
}

TEST(Peg, StringFailure)
{
    using Parser = peg::String<'a','b'>;
    string str = "ac";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, StringCaselessSuccess)
{
    using Parser = peg::StringCaseless<'a','b'>;
    string str = "aB";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "aB");
}

TEST(Peg, StringCaselessFailure)
{
    using Parser = peg::StringCaseless<'a','b'>;
    string str = "ac";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



