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
    template<template<class> class... Actions, class... States>
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
    template<template<class> class... Actions, class... States>
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
struct PrefixZeroOrMore : Sequence<Prefix, ZeroOrMore<Regular>> {};

template<class Prefix, class Regular>
struct PrefixOneOrMore : Sequence<Prefix, OneOrMore<Regular>> {};

struct Digit : Range<'0','9'> {};
struct NonZeroDigit : Range<'1','9'> {};
struct DecimalConstant : PrefixZeroOrMore<NonZeroDigit, Digit> {};

struct OctalDigit : Range<'0','7'> {};
struct OctalConstant : PrefixZeroOrMore<Character<'0'>, OctalDigit> {};

struct HexadecimalDigit : Range<'0','9','a','f','A','F'> {};
struct HexadecimalPrefix : Sequence<Character<'0'>, Choice<Character<'x'>, Character<'X'>>> {};
struct HexadecimalConstant : Sequence<HexadecimalPrefix, OneOrMore<HexadecimalDigit>> {};

struct HexQuad: Sequence<HexadecimalDigit, HexadecimalDigit, HexadecimalDigit, HexadecimalDigit>{};
struct UniversalCharacterName : Sequence<c::Backslash, Range<'u','u','U','U'>, HexQuad> {};

struct IdentifierDigit : Choice<Range<'a','z','A','Z','_','_','0','9'>, UniversalCharacterName> {};
struct IdentifierNonDigit : Choice<Range<'a','z','A','Z','_','_'>, UniversalCharacterName> {};
struct Identifier : Sequence<IdentifierNonDigit, ZeroOrMore<IdentifierDigit>> {};

int tool_main(int argc, const char *argv[])
{
    core::POpt opts;
    opts.process(argc, argv);
    cout << std::boolalpha;

    for (auto input : opts.extra())
    {
	auto state = Environment{};
	auto r = parse<Identifier, DebugAction>(input, state);
	cout << input << ": " << (bool)r << " " << r.match() << endl;
    }
    return 0;
}

