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

struct Fact : LeftRecursion<
    Or<
	Seq<Fact, c::Multiply, Number>,
	Seq<Fact, c::Divide, Number>,
	Number>>
{};

struct Expr : LeftRecursion<
    Or<
	Seq<Expr, c::Plus, Fact>,
	Seq<Expr, c::Minus, Fact>,
	Fact>>
{};

int tool_main(int argc, const char *argv[])
{
    auto opts = ArgParse
	(
	 argValues<'*',vector,string>("", "", 1)
	 );
    opts.parse(argc, argv);

    for (auto& str : opts.get<'*'>())
    {
	auto r = parse<Expr>(str);
	cout << str << " : " << r.status() << " : " << r.match() << endl;
    }

    // {
    // 	// [[maybe_unused]] expr::check_recursion_t<Expr> ignore;
    // 	expr::apply_rt<expr::printer, std::tuple<Expr>>::apply(cout);
    // }

    return 0;
}

