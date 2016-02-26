//==================================================================================================
// Brief   : Tuple Utilities
// -------------------------------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2012 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_TUPLE_UTILITY__HPP_
#define UL_TUPLE_UTILITY__HPP_

////////////////////////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>

#include <utility>

////////////////////////////////////////////////////////////////////////////////////////////////////
namespace ul {

////////////////////////////////////////////////////////////////////////////////////////////////////
template<size_t I, size_t ...Indexes>
struct make_index_sequence : make_index_sequence<I-1, I-1, Indexes...> { };

template<size_t ...Indexes>
struct make_index_sequence<0, Indexes...> { typedef std::index_sequence<Indexes...> type; };

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class F, class Tuple, size_t... Indexes>
inline decltype(auto) unwrap_and_invoke(F&& f, Tuple&& args, std::index_sequence<Indexes...>)
{
	return f(std::get<Indexes>(std::forward<Tuple>(args))...);
}

template<class F, class Tuple>
inline decltype(auto) unwrap_and_invoke(F&& f, Tuple&& args)
{
	return unwrap_and_invoke(std::forward<F>(f),
	                         std::forward<Tuple>(args),
	                         make_index_sequence<std::tuple_size<typename std::decay<Tuple>::type>::value>::type());
}

////////////////////////////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

////////////////////////////////////////////////////////////////////////////////////////////////////
#endif /* UL_TUPLE_UTILITY__HPP_ */
