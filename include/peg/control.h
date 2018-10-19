// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "peg/detect.h"
#include "peg/input.h"

namespace peg
{

template<class T>
constexpr std::string_view get_name()
{
    char const* p = __PRETTY_FUNCTION__;
    
    while (*p and *p != '=')
	++p;
    if (*p)
	++p;

    while (*p and *p == ' ')
	++p;

    auto begin = p;
    while (*p and *p != ';')
	++p;

    return { begin, std::size_t(p - begin) };
}

template<typename Parser>
struct DebugAction
{
    using Type = DebugAction<Parser>;
    static void apply(const Input& input)
    {
	auto view = get_name<Type>();
	cout << string(view) << ": ";
	cout << input.match() << endl;
    }
};

template<typename Parser,
	 template <typename...> typename Control,
	 template <typename...> typename... Actions>
struct Machine
{
    template<typename... States>
    static Input match(const Input& input, States&... states)
    {
	if constexpr (is_detected<has_start_method, Control<Parser>>::value)
			 Control<Parser>::start(input, states...);

	auto r = Parser::template match<Control,Actions...>(input, states...);
	
	if (r)
	{
	    (Actions<Parser>::apply(r, states...), ...);
	    if constexpr (is_detected<has_success_method, Control<Parser>>::value)
	     		     Control<Parser>::success(input, states...);
	}
	else
	{
	    if constexpr (is_detected<has_failure_method, Control<Parser>>::value)
	     		     Control<Parser>::failure(input, states...);
	}

	return r;
    }
};

template<typename Parser>
struct NormalControl
{ };

}; // end peg
