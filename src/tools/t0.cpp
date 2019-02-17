// Copyright (C) 2019 by Mark Melton
//

#include <fmt/format.h>
#include <stack>
#include "core/common.h"
#include "core/demangle.h"
#include "peg/peg.h"

using namespace peg;

string name;

template<class P, size_t ID>
struct MatchPush
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    { return Control::template match<P, Actions...>(input, states...); }
};

template<class P, size_t ID>
struct MatchPop
{
    template<template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    { return Control::template match<P, Actions...>(input, states...); }
};

struct Name : OneOrMore<Range<'a','z'>> {};
struct StartTag : Sequence<c::LessThan, MatchPush<Name,5>, c::GreaterThan> {};
struct EndTag : Sequence<c::LessThan, c::ForwardSlash, MatchPop<Name,5>, c::GreaterThan> {};

struct Content;
struct Element : Sequence<StartTag, Content, EndTag> {};
struct Content : Choice<Element, OneOrMore<Range<'a','z'>>> {};

struct State
{
    State()
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

template<class Rule>
struct Env : NullAction {};

template<>
struct Env<MatchPush<Name,5>>
{
    static void apply(const Input& input, State& state)
    {
	cout << "push: " << input.match() << endl;
	state.template push<5>(input.match_view());
    }
};

template<>
struct Env<MatchPop<Name,5>>
{
    static void apply(const Input& input, State& state)
    {
	auto match = state.template top<5>();
	cout << match << " " << input.match() << endl;
    }
};

int main(int argc, const char *argv[])
{
    cout << std::boolalpha;
    auto input = "<abc>hello</abc>"s;
    auto state = State{};
    auto r = parse<Content, Env>(input, state);
    cout << (bool)r << " " << r.match() << endl;
    return 0;
}

