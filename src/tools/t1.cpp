// Copyright (C) 2019 by Mark Melton
//

#include <fmt/format.h>
#include <stack>
#include "core/tool.h"
#include "core/demangle.h"
#include "peg/peg.h"

using namespace peg;

struct Environment
{
    Environment()
	: env(10)
    { }

    template<size_t ID>
    void push(string_view s) { env[ID].push(s); }

    template<size_t ID>
    auto top() { return env[ID].top(); }

    template<size_t ID>
    string_view pop()
    {
	auto r = env[ID].top();
	env[ID].pop();
	return r;
    }
	
    vector<std::stack<string_view>> env;
};

template<class P, size_t ID>
struct MatchPush
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, Environment& env, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, env, states...);
	if (not r) return r;

	env.template push<ID>(r.match_view());
	return r;
    }
};

template<class P, size_t ID>
struct MatchPop
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, Environment& env, States&... states)
    {
	auto r = Control::template match<P, Actions...>(input, env, states...);
	if (not r) return r;

	auto str = env.template top<ID>();
	if (str != r.match_view())
	    return input.failure();
	env.template pop<ID>();
	return r;
    }
};

template<class Prefix, class Regular>
struct PrefixZeroPlus : Sequence<Prefix, ZeroOrMore<Regular>> {};

template<class Prefix, class Regular>
struct PrefixOnePlus : Sequence<Prefix, OneOrMore<Regular>> {};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);
    cout << std::boolalpha;

    // for (auto input : opts.extra())
    // {
    // 	auto state = Environment{};
    // 	auto r = parse<Identifier, DebugAction>(input, state);
    // 	cout << input << ": " << (bool)r << " " << r.match() << endl;
    // }
    return 0;
}

