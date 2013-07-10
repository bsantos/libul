//=============================================================================
// Brief : Unicode Utils
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_UNICODE__HPP_
#define UL_UNICODE__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/exception.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace ul {

////////////////////////////////////////////////////////////////////////////////
struct invalid_utf8_encoding : virtual public ul::exception {
	char const* what() const throw();
};

struct invalid_utf16_encoding : virtual public ul::exception {
	char const* what() const throw();
};

////////////////////////////////////////////////////////////////////////////////
static constexpr uint k_uc_replacement_char = 0x0000fffd;
static constexpr uint k_uc_max_utf16        = 0x0010ffff;
static constexpr uint k_uc_max_char         = 0x7fffffff;
static constexpr uint k_uc_invalid_char     = k_uc_max_char + 1;

////////////////////////////////////////////////////////////////////////////////
uint uc8_to_uc32(char const*& begin, char const* end, nothrow_t) noexcept;

inline uint uc8_to_uc32(char const*& begin, char const* end)
{
	uint cp = uc8_to_uc32(begin, end, nothrow);
	if (UL_UNLIKELY(cp == k_uc_invalid_char))
		throw_exception(invalid_utf8_encoding());

	return cp;
}

uint uc16_to_uc32(ushort const*& begin, ushort const* end, nothrow_t) noexcept;

inline uint uc16_to_uc32(ushort const*& begin, ushort const* end)
{
	uint cp = uc16_to_uc32(begin, end);
	if (cp == k_uc_invalid_char)
		throw_exception(invalid_utf16_encoding());

	return cp;
}

constexpr ushort uc16_high_surrogate(uint cp)
{
	return 0xd800 + ((cp - 0x10000) >> 10);
}

constexpr ushort uc16_low_surrogate(uint cp)
{
	return 0xdc00 + ((cp - 0x10000) & 0x3ff);
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_UNICODE__HPP_ */
