// Copyright (C) 2019 by Mark Melton
//

#include <typeindex>
#include <stack>
#include <fmt/format.h>
#include "core/tool.h"
#include "core/demangle.h"
#include "peg/character.h"
#include "peg/sequence.h"

using namespace peg;

struct MyControl
{
    template<typename Parser, template<typename> typename... Actions, typename... States>
    static Input match(Input input, States&... states)
    {
	static const char *last_point{nullptr};
	auto p = input.point();
	cout << size_t(p) << " " << size_t(last_point) << endl;
	
	if (p == last_point)
	    return input.failure();
	last_point = p;
	
	auto r = Parser::template match<MyControl, Actions...>(input, states...);
	r.mark(p);
	
	if (not r) return r;
	(Actions<Parser>::apply(r, states...) , ...);
	return r;
    }

    template<class T>
    static void raise()
    {
	throw std::runtime_error("Control::raise");
    }
};

template<typename Parser>
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

struct Expr : Or<
    Seq<Expr, c::Plus, c::a>,
    c::a
    >
{};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);

    cout << sizeof(typeid(Expr)) << endl;
    cout << sizeof(std::type_index(typeid(Expr))) << endl;

    string str = "a+a";
    Input input(str);
    auto r = MyControl::template match<Expr, DebugAction>(input);
    cout << r.match() << endl;
    return 0;
}

