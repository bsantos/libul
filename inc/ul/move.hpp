//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2009-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_MOVE__HPP_
#define UL_MOVE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/type_traits/remove_reference.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T>
class rv_ {
	typedef typename boost::remove_reference<T>::type type;

	type& _ref;

public:
    explicit rv_(type& ref)
        : _ref(ref)
    { }

    T* operator->() { return &_ref; }
    T* operator&()  { return &_ref; }
};

template<class T>
inline T move(T& from)
{
	return T(rv_<T>(from));
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#endif /* UL_MOVE__HPP_ */
