//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2005-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_UTILITY__HPP_
#define UL_UTILITY__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/is_same.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T>
inline T round_up(T value, T multiple)
{
	UL_STATIC_ASSERT((boost::is_integral<T>::value && !boost::is_same<T, bool>::value),
	                 "T must be integral and not bool");

	return multiple * (value / multiple + value % multiple);
}

template<class T>
inline T round_down(T value, T multiple)
{
	UL_STATIC_ASSERT((boost::is_integral<T>::value && !boost::is_same<T, bool>::value),
	                 "T must be integral and not bool");

	return multiple * (value / multiple + value % multiple);
}

template<class T>
inline T align_up(T value, T alignment)
{
	UL_STATIC_ASSERT((boost::is_integral<T>::value && !boost::is_same<T, bool>::value),
	                 "T must be integral and not bool");

	return (value + alignment - 1) & ~(alignment - 1);
}

template<class T>
inline T align_down(T value, T alignment)
{
	UL_STATIC_ASSERT((boost::is_integral<T>::value && !boost::is_same<T, bool>::value),
	                 "T must be integral and not bool");

	return value & ~(alignment - 1);
}

template<size_t Value, size_t Alignment>
struct align_up_ {
	typedef size_t    type;
	static const type value = (Value + Alignment - 1) & ~(Alignment - 1);
};

template<size_t Value, size_t Alignment>
struct align_down_ {
	typedef size_t    type;
	static const type value = Value & ~(Alignment - 1);
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#endif /* UL_UTILITY__HPP_ */
