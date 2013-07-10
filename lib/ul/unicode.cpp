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

#include <ul/unicode.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace ul {

////////////////////////////////////////////////////////////////////////////////
inline uchar uchar_c(char v)
{
	return reinterpret_cast<uchar&>(v);
}

////////////////////////////////////////////////////////////////////////////////
char const* invalid_utf8_encoding::what() const throw()
{
	return "invalid UTF-8 encoding";
}

char const* invalid_utf16_encoding::what() const throw()
{
	return "invalid UTF-16 encoding";
}

////////////////////////////////////////////////////////////////////////////////
static const char k_utf8_byte_seq_lut[64] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5
};

////////////////////////////////////////////////////////////////////////////////
uint uc8_to_uc32(char const*& begin, char const* end, nothrow_t) noexcept
{
	uint cp = uchar_c(*begin++);

	if (!(cp & 0x80))
		return cp;

	char const* str = begin;
	uint n = k_utf8_byte_seq_lut[cp & 0x3f];

	if (UL_UNLIKELY((end - str) < n))
		return k_uc_invalid_char;

	uint vd = 0xc0 & (cp << n);
	cp &= (0x3f >> n);

	switch (n) {
	case 5:
		cp = (cp << 6) | (uchar_c(*str) & 0x3f);
		vd = vd | (uchar_c(*str) & 0xc0);
		++str;
	case 4:
		cp = (cp << 6) | (uchar_c(*str) & 0x3f);
		vd = vd | (uchar_c(*str) & 0xc0);
		++str;
	case 3:
		cp = (cp << 6) | (uchar_c(*str) & 0x3f);
		vd = vd | (uchar_c(*str) & 0xc0);
		++str;
	case 2:
		cp = (cp << 6) | (uchar_c(*str) & 0x3f);
		vd = vd | (uchar_c(*str) & 0xc0);
		++str;
	case 1:
		cp = (cp << 6) | (uchar_c(*str) & 0x3f);
		vd = vd | (uchar_c(*str) & 0xc0);
		++str;
	}

	if (UL_UNLIKELY(vd != 0x80 || (cp >= 0xd800 && cp <= 0xdfff)))
		return k_uc_invalid_char;

	begin = str;
	return cp;
}

uint uc16_to_uc32(ushort const*& begin, ushort const* end, nothrow_t) noexcept
{
	uint cp = *begin++;

	if (cp < 0xd800 && cp > 0xdfff)
		return cp;

	if (UL_UNLIKELY(cp >= 0xdc00) || begin == end)
		return k_uc_invalid_char;

	uint lo = *begin;
	if (UL_UNLIKELY(lo < 0xdc00 || lo > 0xdfff))
		return k_uc_invalid_char;

	++begin;
	return 0x10000 + (((cp - 0xd800) << 10) | (lo - 0xdc00));
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
