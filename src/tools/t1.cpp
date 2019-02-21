// Copyright (C) 2019 by Mark Melton
//

#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "core/string/replace.h"
#include "peg/peg.h"
#include "peg/expr/expr.h"

using namespace peg;

struct Number : Range<'0','9'> {};
struct Numbers : OneOrMore<Range<'0','9'>> {};

struct Fact;
struct Fact : Or<
    Seq<Number, c::Multiply, Number>,
    Seq<Number, c::Divide, Number>,
    Number>
{};

struct Expr;
struct Expr : Or<
    Seq<Expr, c::Plus, Fact>,
    Seq<Expr, c::Minus, Fact>,
    Fact>
{};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    {
	struct e;
	struct e :  Or<
	    Seq<e, c::Multiply, Number>,
	    Seq<Number, c::Divide, Number>,
	    Number>
	{};
	
	expr::apply_rt<expr::printer, std::tuple<e>>::apply(cout);
    }

    return 0;
}

