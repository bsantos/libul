//=============================================================================
// Brief : Cast Utilities
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2009-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_CAST__HPP_
#define UL_CAST__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/type_traits/is_integral.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T, class U, bool Check>
struct cast_impl;

template<class T, class U>
struct cast_impl<T, U, true> {
	static T truncate(U from)
	{
		if (static_cast<U>(static_cast<T>(from)) != from)
			UL_BREAKPOINT;

		return static_cast<T>(from);
	}
};

template<class T, class U>
struct cast_impl<T, U, false> {
	static T truncate(U from)
	{
		return static_cast<T>(from);
	}
};

template<class T, class U>
inline T truncate_cast(U from)
{
	UL_STATIC_ASSERT(boost::is_integral<T>::value
	                 && boost::is_integral<U>::value, "T and U must be integral types");

	typedef typename cast_impl<T,
	                           U,
	                           boost::mpl::less<boost::mpl::sizeof_<T>,
	                                            boost::mpl::sizeof_<U>
	                                           >::value
	                          > impl;

	return impl::truncate(from);
}

///////////////////////////////////////////////////////////////////////////////
template<class T, class U>
inline T* offset_cast(U* from, size_t offset)
{
	return reinterpret_cast<T*>(reinterpret_cast<uchar*>(from) + offset);
}

template<class T, class U>
inline T const* offset_cast(U const* from, size_t offset)
{
	return reinterpret_cast<T const*>(reinterpret_cast<uchar const*>(from) + offset);
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_CAST__HPP_ */
