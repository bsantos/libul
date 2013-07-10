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

#include <ul/gfx/font.hpp>
#include <CoreFoundation/CFString.h>
#include <CoreText/CTFont.h>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
typedef nsobj_ref<__CFString const> cf_string_ref;

////////////////////////////////////////////////////////////////////////////////
font::font(string_ref const& name, float_type size)
	: _font(CTFontCreateWithName(
	            cf_string_ref(CFStringCreateWithCStringNoCopy(kCFAllocatorDefault,
	                                                    name.data(),
	                                                    kCFStringEncodingUTF8,
	                                                    kCFAllocatorNull)).get(),
	                                                    size, nullptr))
{ }

float_type font::size() const
{
	return CTFontGetSize(_font.get());
}

float_type font::ascent() const
{
	return CTFontGetAscent(_font.get());
}

float_type font::descent() const
{
	return CTFontGetDescent(_font.get());
}

float_type font::leading() const
{
	return CTFontGetLeading(_font.get());
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
