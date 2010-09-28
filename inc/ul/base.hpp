//=============================================================================
// Brief : Base Types and Definitions
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2005-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_BASE__HPP_
#define UL_BASE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/static_assert.hpp>

///////////////////////////////////////////////////////////////////////////////
#define UL_SCOPED(def)     switch(def) case 0: default:
#define UL_COUNT_OF(array) (sizeof(array) / sizeof(array[0]))

#ifdef BOOST_HAS_STATIC_ASSERT
#	define UL_STATIC_ASSERT(exp, reason) static_assert(exp, reason)
#else
#	define UL_STATIC_ASSERT(exp, reason) BOOST_STATIC_ASSERT(exp)
#endif

#if defined(__GNUC__)
#	define UL_LIKELY(x) __builtin_expect(!!(x), 1)
#else
#	define UL_LIKELY(x) (x)
#endif

#if defined(__GNUC__)
#	define UL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#	define UL_UNLIKELY(x) (x)
#endif

#if defined(__GNUC__)
#	define UL_RETURN_ADDRESS __builtin_return_address(0)
#elif defined(BOOST_MSVC)
	extern "C" void* _ReturnAddress();

#	define UL_RETURN_ADDRESS _ReturnAddress()
#else
#	define UL_RETURN_ADDRESS 0
#endif

#if defined(__GNUC__)
#	define UL_DEPRECATE __attribute__((deprecated))
#elif defined(BOOST_MSVC)
#	define UL_DEPRECATE __declspec(deprecated)
#else
#	define UL_DEPRECATE
#endif

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
typedef signed char        schar;
typedef signed short       sshort;
typedef signed int         sint;
typedef signed long        slong;
typedef signed long long   sllong;

typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef unsigned long      ulong;
typedef unsigned long long ullong;

typedef boost::uint8_t     uint8;
typedef boost::uint16_t    uint16;
typedef boost::uint32_t    uint32;
typedef boost::uint64_t    uint64;

typedef boost::int8_t      sint8;
typedef boost::int16_t     sint16;
typedef boost::int32_t     sint32;
typedef boost::int64_t     sint64;

typedef boost::intmax_t    sintmax;
typedef boost::uintmax_t   uintmax;

///////////////////////////////////////////////////////////////////////////////
struct nullptr_t { template<class T> operator T*() const { return 0; } };

static const nullptr_t nullptr = nullptr_t();

///////////////////////////////////////////////////////////////////////////////
template<class MemberT, class ParentT>
inline MemberT* member_of(ParentT* parent, MemberT ParentT::* Member)
{
	return &(parent->*Member);
}

template<class ParentT, class MemberT>
inline ParentT* parent_of(MemberT* member, MemberT ParentT::* Member)
{
	ParentT* parent = 0;

	if (!member)
		return nullptr;

	return reinterpret_cast<ParentT*>(reinterpret_cast<char*>(member) - reinterpret_cast<const char*>(&(parent->*Member)));
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_BASE__HPP_ */
