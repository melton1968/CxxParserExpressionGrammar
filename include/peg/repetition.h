// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"
#include "peg/sequence.h"

namespace peg
{

template<size_t N, size_t M, class... Ps>
struct Repetition
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	return Repetition<N, M, Sequence<Ps...>>::template match<Control, Actions...>
	    (input, states...); }
};

template<size_t N, size_t M, class P>
struct Repetition<N, M, P>
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	size_t count = 0;
	auto last_r = input;
	Input r = input;
	while (count < M and (r = P::template match<Control, Actions...>(last_r, states...)))
	{
	    ++count;
	    last_r = r;
	}
	if (count >= N)
	    return last_r;
	else
	    return input.failure();
    }
};

template<class... Ps> using ZeroOrOne = Repetition<0, 1, Ps...>;
template<class... Ps> using Maybe = Repetition<0, 1, Ps...>;
template<class... Ps> using ZeroOrMore = Repetition<0, std::numeric_limits<size_t>::max(), Ps...>;
template<class... Ps> using OneOrMore = Repetition<1, std::numeric_limits<size_t>::max(), Ps...>;

template<size_t N, class... Ps>
using AtLeast = Repetition<N, std::numeric_limits<size_t>::max(), Ps...>;

template<class Parser, class Infix, class Ignore>
using List = Sequence<Parser,ZeroOrMore<Ignore,Infix,Parser>,Maybe<Infix>>;

template<class Parser, class Infix, class Ignore>
using InfixList = Sequence<Parser,ZeroOrMore<Ignore,Infix,Parser>>;

}; // end peg
