// Copyright (C) 2019 by Mark Melton
//

#include <typeindex>
#include <stack>
#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "peg/peg.h"

using namespace peg;

struct Result
{
    bool valid{false};
    Input input;
};

template<class P>
struct Recurse
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	static std::map<const char*, const char*> seeds;

	auto begin = input.point();
	auto iter = seeds.find(begin);
	if (iter != seeds.end())
	    return input.with_status(iter->second != begin, iter->second - begin);

	seeds[begin] = begin;

	auto best = input;
	while (true)
	{
	    auto r = Control::template match<P, Actions...>(input, states...);	    
	    if (r and r.point() > seeds[begin])
	    {
		best = r;
		seeds[begin] = r.point();
	    }
	    else
	    {
		seeds.erase(begin);
		return best;
	    }
	}
    }
};

struct MyControl
{
    template<class Parser, template<class> class... Actions, class... States>
    static Input match(Input input, States&... states)
    {
	cout << "pre:   ";
	(Actions<Parser>::apply(input, states...) , ...);
	
	auto p = input.point();
	auto r = Parser::template match<MyControl, Actions...>(input, states...);
	r.mark(p);
	
	cout << "post   ";
	(Actions<Parser>::apply(r, states...) , ...);

	if (not r) return r;
	cout << "match  ";
	(Actions<Parser>::apply(r, states...) , ...);
	return r;
    }

    template<class T>
    static void raise()
    {
	throw std::runtime_error("Control::raise");
    }
};

template<class Parser>
struct DebugAction
{
    using Type = DebugAction<Parser>;
    template<class... States>
    static void apply(const Input& input, States&...)
    {
	auto view = core::type_name<Type>();
	cout << string(view) << ": ";
	cout << input.match() << endl;
    }
};

struct Expr : Recurse<
    Or<
	Seq<Expr, c::Plus, c::a>,
	c::a
	>
    >
{};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    cout << sizeof(typeid(Expr)) << endl;
    cout << sizeof(std::type_index(typeid(Expr))) << endl;

    string str = "a+a+a+a";
    Input input(str);
    auto r = MyControl::template match<Expr, ::DebugAction>(input);
    cout << r.match() << endl;
    return 0;
}

