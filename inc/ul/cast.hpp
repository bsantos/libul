//=============================================================================
// Brief : Cast Utilities
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2009-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_CAST__HPP_
#define UL_CAST__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/exception.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/sizeof.hpp>
#include <boost/type_traits/is_integral.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
struct bad_cast : virtual public exception {
};

struct bad_truncate_cast : virtual public bad_cast {
	bad_truncate_cast() : exception("ul::truncate_cast: bad cast")
	{ }
};

///////////////////////////////////////////////////////////////////////////////
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template<class T, class U, bool Check>
struct cast;

template<class T, class U>
struct cast<T, U, true> {
	static T truncate(U from)
	{
		if (U(T(from)) != from)
			boost::throw_exception(bad_truncate_cast());

		return T(from);
	}
};

template<class T, class U>
struct cast<T, U, false> {
	static T truncate(U from)
	{
		return T(from);
	}
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace detail */

///////////////////////////////////////////////////////////////////////////////
template<class T, class U>
inline T truncate_cast(U from)
{
	UL_STATIC_ASSERT(boost::is_integral<T>::value
						 && boost::is_integral<U>::value, "T and U must be an integral type");

	typedef typename detail::cast<T,
								  U,
								  boost::mpl::less<boost::mpl::sizeof_<T>,
												   boost::mpl::sizeof_<U>
												  >::value
								 > impl;

	return impl::truncate(from);
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_CAST__HPP_ */
