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

///////////////////////////////////////////////////////////////////////////////
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#	define UL_LITTLE_ENDIAN
#endif

#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
#	define UL_BIG_ENDIAN
#endif

#if  (defined(UL_LITTLE_ENDIAN) && defined(UL_BIG_ENDIAN)) || \
	!(defined(UL_LITTLE_ENDIAN) || defined(UL_BIG_ENDIAN))
#	error "Unable to determine endianess, you must support/fix it"
#endif

#if defined(__clang__) || defined(__GCC__)
	#define UL_LIKELY(exp)   __builtin_expect(static_cast<bool>(exp), true)
	#define UL_UNLIKELY(exp) __builtin_expect(static_cast<bool>(exp), false)
#else
	#define UL_LIKELY(exp)   static_cast<bool>(exp)
	#define UL_UNLIKELY(exp) static_cast<bool>(exp)
#endif

///////////////////////////////////////////////////////////////////////////////
//deprecated: use count_of or std::begin/std::end
#define UL_COUNT_OF(array) (sizeof(array) / sizeof(array[0]))
#define UL_CONCAT_(a, b)   a ## b
#define UL_CONCAT(a, b)    UL_CONCAT_(a, b)

//deprecated: use C++11 static_assert
#define UL_STATIC_ASSERT(exp, reason) static_assert(exp, reason)

#if defined(__GNUC__)
#	define UL_BREAKPOINT __builtin_trap()
#else
#	define UL_BREAKPOINT *(volatile char *)0xbadcafe = 0
#endif

//deprecated: use C++11 explicit operator bool
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
constexpr bool is_little_endian()
{
#ifdef UL_LITTLE_ENDIAN
	return true;
#else
	return false;
#endif
}

constexpr bool is_big_endian()
{
#ifdef UL_BIG_ENDIAN
	return true;
#else
	return false;
#endif
}

template<class T, size_t N>
constexpr size_t count_of(T (&)[N])
{
	return N;
}

template<class Member, class Parent>
constexpr Member* member_of(Parent* parent, Member Parent::* MemberPtr)
{
	return &(parent->*MemberPtr);
}

template<class Parent, class Member>
constexpr Parent* parent_of(Member* member, Member Parent::* MemberPtr)
{
	return (member) ? reinterpret_cast<Parent*>(
	                        reinterpret_cast<char const volatile*>(member)
	                      - reinterpret_cast<char const volatile*>(
	                            member_of(static_cast<Parent*>(nullptr), MemberPtr)
	                        )
	                  )
	                : nullptr;
}


struct nothrow_t { };

static constexpr nothrow_t nothrow = nothrow_t();

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

///////////////////////////////////////////////////////////////////////////////
#endif /* UL_BASE__HPP_ */
