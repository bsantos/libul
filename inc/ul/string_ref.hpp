//=============================================================================
// Brief : Immutable String Reference
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_STRING_REF__HPP_
#define UL_STRING_REF__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <string>

////////////////////////////////////////////////////////////////////////////////
namespace ul {

////////////////////////////////////////////////////////////////////////////////
template<class Char, class Traits>
class basic_string_ref {
public:
	constexpr basic_string_ref()
		: _str(nullptr), _len(0)
	{ }

	constexpr basic_string_ref(Char const* str, size_t len)
		: _str(str), _len(len)
	{ }

	basic_string_ref(Char const* str)
		: _str(str), _len(Traits::length(str))
	{ }

	template<class Allocator>
	basic_string_ref(std::basic_string<Char, Traits, Allocator> const& rhs)
		: _str(rhs.data()), _len(rhs.length())
	{ }

	constexpr Char const* data() const   { return _str; }
	constexpr size_t      length() const { return _len; }

	constexpr Char const& operator[](size_t idx) const { return _str[idx]; }

	constexpr Char const* begin() const { return _str; }
	constexpr Char const* end() const   { return _str + _len; }

	constexpr Char const* cbegin() const { return _str; }
	constexpr Char const* cend() const   { return _str + _len; }

	constexpr explicit operator bool() const { return _len; }

private:
	Char const* _str;
	size_t      _len;
};

typedef basic_string_ref<char, std::char_traits<char>> string_ref;

////////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_STRING_REF__HPP_ */
