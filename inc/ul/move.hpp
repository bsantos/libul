//=============================================================================
// Brief : Move Emulation
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2009-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_MOVE__HPP_
#define UL_MOVE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_convertible.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T>
struct move_ : public T {
	typedef move_<T> self_type;

	move_(const self_type& r)
		: T(const_cast<self_type&>(r))
	{ }
};

template<class T>
inline typename boost::enable_if<boost::is_convertible<move_<T>, T>, move_<T>&>::type move(T& from)
{
	return static_cast<move_<T>&>(from);
}

///////////////////////////////////////////////////////////////////////////////
#define UL_MOVABLE_BUT_NOT_COPYABLE(TYPE)            \
	private:                                         \
	TYPE(TYPE &);                                    \
	TYPE& operator=(TYPE &);                         \
	public:                                          \
	operator ul::move_<TYPE>&()                      \
	{ return static_cast<ul::move_<TYPE>&>(*this); } \
	private:

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_MOVE__HPP_ */
