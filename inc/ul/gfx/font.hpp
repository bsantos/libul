//=============================================================================
// Brief : GFX Font
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_FONT__HPP_
#define UL_GFX_FONT__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/nsobj.hpp>
#include <ul/string_ref.hpp>
#include <ul/gfx/types.hpp>

////////////////////////////////////////////////////////////////////////////////
#ifdef __APPLE__
	typedef struct __CTFont const* CTFontRef;
#endif

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
namespace detail {
#ifdef __APPLE__
	typedef CTFontRef                 font_type;
	typedef nsobj_ref<__CTFont const> font_ref;

	inline font_ref font_retain(font_type ref)
	{
		if (ref)
			ref = (font_type) CFRetain(ref);
		return font_ref(ref);
	}

	inline font_ref font_retain(font_ref const& ref)
	{
		return font_retain(ref.get());
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////
class font {
public:
	typedef detail::font_type native_type;

public:
	font() = default;
	font(font&&) = default;
	font& operator=(font&&) = default;
	font(font const& f);
	font& operator=(font const& f);

	explicit font(native_type native);
	font(string_ref const& name, float_type size);

	float_type size() const;
	float_type ascent() const;
	float_type descent() const;
	float_type leading() const;

	explicit operator bool() const   { return static_cast<bool>(_font); }
	native_type       native() const { return _font.get(); }

private:
	detail::font_ref _font;
};

inline font::font(native_type native)
	: _font(detail::font_retain(native))
{ }

inline font::font(font const& f)
	: _font(detail::font_retain(f._font))
{ }

inline font& font::operator=(font const& f)
{
	if (this != &f)
		_font = detail::font_retain(f._font);
	return *this;
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_FONT__HPP_ */
