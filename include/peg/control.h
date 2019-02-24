// Copyright (C) 2018, 2019 by Mark Melton
//

#pragma once
#include "core/demangle.h"
#include "peg/action.h"
#include "peg/input.h"
#include "peg/whitespace.h"

namespace peg
{

template<class Parser>
struct DebugAction : NullAction<Parser>
{
    using Type = DebugAction<Parser>;
    template<class... States>
    static void success(const Input& input, States&&...)
    {
	auto view = core::type_name<Type>();
	cout << string(view) << ": ";
	cout << input.match() << endl;
    }
};

template<class Skip = void>
struct BasicControl
{
    using Self = BasicControl;
    using SkipParser = Skip;

    template<class NewSkip>
    using WithNewSkip = BasicControl<NewSkip>;
	
    template<class Parser, template<class> class... Actions, class... States>
    static Input match(Input input, States&&... states)
    {
	if constexpr (not std::is_same_v<SkipParser, void>)
			 input = SkipParser::template match<Self>(input);
	
	(Actions<Parser>::start(input, states...) , ...);
	
	auto r = Parser::template match<Self, Actions...>(input, states...);
	r.mark(input.point());

	bool valid = (Actions<Parser>::validate(r, states...) and ...);

	auto final_r = valid ? r : input.failure();
	if (final_r) (Actions<Parser>::success(r, states...) , ...);
	else (Actions<Parser>::failure(final_r, states...), ...);
	
	return final_r;
    }

    template<class T>
    static void raise()
    {
	throw std::runtime_error("Control::raise");
    }
};

template<class Skip, class Parser>
struct ChangeSkip
{
    template<class Control, template<class> class... Actions, class... States>
    static Input match(Input input, States&... states)
    {
	using NewControl = typename Control:: template WithNewSkip<Skip>;
	return NewControl::template match<Parser, Actions...>(input, states...);
    }
};

template<class P>
using NoSkip = ChangeSkip<void, P>;

template<class P>
using SkipWhiteSpace = ChangeSkip<WhiteSpace, P>;

}; // end peg
