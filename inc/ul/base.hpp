//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2005-2013 Bruno Santos <bsantos@cppdev.net>
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
#define UL_COUNT_OF(array) (sizeof(array) / sizeof(array[0]))
#define UL_CONCAT_(a, b)   a ## b
#define UL_CONCAT(a, b)    UL_CONCAT_(a, b)

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
#	define UL_BREAKPOINT __builtin_trap()
#else
#	define UL_BREAKPOINT *(volatile char *)0 = *(volatile char *)0
#endif

#define UL_UNDEFINED_BOOL                              \
	struct undefined_bool_t {                          \
		void true_() {}                                \
	};                                                 \
	typedef void (undefined_bool_t::*undefined_bool)()

#define UL_UNDEFINED_BOOL_TRUE  &undefined_bool_t::true_
#define UL_UNDEFINED_BOOL_FALSE 0

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

typedef intptr_t  sintptr;
typedef uintptr_t uintptr;

///////////////////////////////////////////////////////////////////////////////
#ifdef BOOST_NO_NULLPTR
	struct nullptr_t { template<class T> operator T*() const { return 0; } };

	static const nullptr_t nullptr = nullptr_t();
#else
	typedef decltype(nullptr) nullptr_t;
#endif

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
template<class T, size_t N>
inline size_t count_of(T (&)[N])
{
	return N;
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#endif /* UL_BASE__HPP_ */
