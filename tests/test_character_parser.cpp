// Copyright (C) 2018 by Mark Melton
//

#include <gtest/gtest.h>
#include "peg/character.h"
#include "peg/parse.h"

using ::testing::StaticAssertTypeEq;

using namespace peg;

TEST(Peg, CharacterSuccess)
{
    using Parser = peg::Character<'a'>;
    auto s = "a"s;
    auto r = parse<Parser>("a");
    EXPECT_TRUE(r);
    EXPECT_EQ(r.match(), "a");
}

TEST(Peg, CharacterFailureEof)
{
    using Parser = peg::Character<'a'>;
    auto r = parse<Parser>("");
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

TEST(Peg, CharacterFailureNoMatch)
{
    using Parser = peg::Character<'a'>;
    auto r = parse<Parser>("b");
    EXPECT_FALSE(r);
    EXPECT_EQ(r.match(), "");
}

// TEST(Peg, NotCharacterSuccess)
// {
//     using Parser = peg::NotCharacter<'a'>;
//     std::string str = "b";
//     auto r = peg::parse<Parser>(str);
//     EXPECT_TRUE(r);
//     EXPECT_EQ(r.match(), "b");
// }

// TEST(Peg, NotCharacterFailure)
// {
//     using Parser = peg::NotCharacter<'a'>;
//     std::string str = "a";
//     auto r = peg::parse<Parser>(str);
//     EXPECT_FALSE(r);
//     EXPECT_EQ(r.match(), "");
// }

// TEST(Peg, CharactersSuccess)
// {
//     using Parser = peg::Characters<'a','b'>;
//     std::string str = "ab";
//     auto r = peg::parse<Parser>(str);
//     EXPECT_TRUE(r);
//     EXPECT_EQ(r.match(), "ab");
// }

// TEST(Peg, CharactersFailure)
// {
//     using Parser = peg::Characters<'a','b'>;
//     std::string str = "ac";
//     auto r = peg::parse<Parser>(str);
//     EXPECT_FALSE(r);
//     EXPECT_EQ(r.match(), "");
// }

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



