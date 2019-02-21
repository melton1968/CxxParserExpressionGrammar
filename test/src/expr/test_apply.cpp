// Copyright (C) 2019 by Mark Melton
//

#include <gtest/gtest.h>
#include "core/demangle.h"
#include "peg/expr/apply.h"

using namespace peg;

template<class T>
struct foo { using type = int; };

template<class T>
using foo_t = typename foo<T>::type;

TEST(PegExpr, Apply)
{
    struct Childless { };
    struct Parent { using Children = std::tuple<Childless>; };
    using T = expr::apply_t<foo_t, Parent>;
    static_assert(std::is_same_v<T, core::mp::list<int>>);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}



