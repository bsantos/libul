//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2009-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_EXCEPTION__HPP_
#define UL_EXCEPTION__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <boost/exception/exception.hpp>
#include <boost/throw_exception.hpp>
#include <exception>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
using boost::throw_exception;

///////////////////////////////////////////////////////////////////////////////
class exception : virtual public std::exception, virtual public boost::exception { };

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_EXCEPTION__HPP_ */
