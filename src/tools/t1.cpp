// Copyright (C) 2019 by Mark Melton
//

#include <fmt/format.h>
#include "core/tool.h"
#include "core/mp/mp.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"

using namespace peg;
namespace mp = core::mp;

struct Number : Range<'0','9'> {};
struct Numbers : OneOrMore<Range<'0','9'>> {};

struct Fact;
struct Fact : Or<
    Seq<LeftRecursion<Fact>, c::Multiply, Number>,
    Seq<LeftRecursion<Fact>, c::Divide, Number>,
    Number>
{};

struct Expr;
struct Expr : Or<
    Seq<LeftRecursion<Expr>, c::Plus, Fact>,
    Seq<LeftRecursion<Expr>, c::Minus, Fact>,
    Fact>
{};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    {
	[[maybe_unused]] expr::check_recursion_t<Expr> ignore;
	expr::apply_rt<expr::printer, std::tuple<Expr>>::apply(cout);
    }

    return 0;
}

