// Copyright (C) 2019 by Mark Melton
//

#pragma once
#include "peg/input.h"

namespace peg
{

template<class S, class Predicate>
struct StringMatcher
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(const Input& input, States&... states)
    {
	auto p = input.point();
	auto q = S::str;
	while (*q and p < input.end())
	{
	    if (not Predicate::apply(*p, *q))
		return input.failure();
	    ++p;
	    ++q;
	}
	index_t count = p - input.point();

	if (count == q - S::str)
	    return input.success(count);
	else
	    return input.failure();
    }
};

namespace s::detail
{
struct CompareWithCase
{ static bool apply(char a, char b) { return a == b; } };

struct CompareWithoutCase
{ static bool apply(char a, char b) { return std::tolower(a) == std::tolower(b); } };
};

}; // end ns peg

#define PEG_SS(S) #S
#define PEG_S(S) PEG_SS(S)

#define PEG_STR(S)							\
    namespace detail {							\
    struct S_ ## S							\
    {									\
	static constexpr const char *str = PEG_S(S);			\
	static constexpr char get(size_t i) { return str[i]; }		\
    };									\
    };									\
    using _ ## S = peg::StringMatcher<detail::S_ ## S, peg::s::detail::CompareWithCase>; \
    using c_ ## S = peg::StringMatcher<detail::S_ ## S, peg::s::detail::CompareWithoutCase>; \
    
