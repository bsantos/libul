//=============================================================================
// Brief : Exception
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2009-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_EXCEPTION__HPP_
#define UL_EXCEPTION__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <boost/exception/exception.hpp>
#include <exception>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
class exception
	: virtual public std::exception, virtual public boost::exception {
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_EXCEPTION__HPP_ */
