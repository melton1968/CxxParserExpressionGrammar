// Copyright (C) 2018 by Mark Melton
//

#pragma once
#include "config.h"

namespace detail
{
template <typename AlwaysVoid, template<typename...> typename Op, typename... Args>
struct detector { using type = std::false_type; };
 
template <template<typename...> typename Op, typename... Args>
struct detector<std::void_t<Op<Args...>>, Op, Args...>
{ using type = std::true_type; };
 
} // end detail

template <template<class...> class Op, class... Args>
using is_detected = typename detail::detector<void, Op, Args...>::type;

template<typename T>
using has_start_method = decltype(&T::start);

// template<typename T, typename... Us>
// using has_start_tmethod = decltype(&T::template start<Us...>);

template<typename T>
using has_success_method = decltype(&T::success);

template<typename T>
using has_failure_method = decltype(&T::failure);
