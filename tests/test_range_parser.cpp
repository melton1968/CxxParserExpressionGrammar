// Copyright (C) 2018 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/character.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

TEST(Peg, CharacterSuccess)
{
    using Parser = peg::Character<'a'>;
    std::string str = "a";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, CharacterFailure)
{
    using Parser = peg::Character<'a'>;
    std::string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, NotCharacterSuccess)
{
    using Parser = peg::NotCharacter<'a'>;
    std::string str = "b";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "b");
}

TEST(Peg, NotCharacterFailure)
{
    using Parser = peg::NotCharacter<'a'>;
    std::string str = "a";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, CharactersSuccess)
{
    using Parser = peg::Characters<'a','b'>;
    std::string str = "ab";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "ab");
}

TEST(Peg, CharactersFailure)
{
    using Parser = peg::Characters<'a','b'>;
    std::string str = "ac";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

inline constexpr char s0[] = "abcdef";

TEST(Peg, StringSuccess)
{
    using Parser = peg::String<s0>;
    std::string str = "abcdef";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "abcdef");
}

TEST(Peg, StringFailure)
{
    using Parser = peg::String<s0>;
    std::string str = "abCdef";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, RangeSuccess)
{
    using Parser = peg::Range<'a','z'>;
    std::string str = "m";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "m");
}

TEST(Peg, RangeFailure)
{
    using Parser = peg::Range<'a','z'>;
    std::string str = "M";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, RangesSuccess)
{
    using Parser = peg::Ranges<'a','z','A','Z'>;
    std::string str = "M";
    auto r = peg::parse<Parser>(str);
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "M");
}

TEST(Peg, RangesFailure)
{
    using Parser = peg::Ranges<'a','z','A','Z'>;
    std::string str = "5";
    auto r = peg::parse<Parser>(str);
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



