//=============================================================================
// Brief : GFX Color
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_COLOR__HPP_
#define UL_GFX_COLOR__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/gfx/types.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
struct color_argb {
	float_type a, r, g, b;
};

constexpr color_argb make_color_argb(float_type a, float_type r, float_type g, float_type b)
{
	return color_argb { a, r, g, b };
}

constexpr color_argb make_color_argb_rgb(float_type r, float_type g, float_type b)
{
	return color_argb { 1., r, g, b };
}

constexpr color_argb make_color_argb_rgb888(uint8 r, uint8 g, uint8 b)
{
	return color_argb { 1., r/255., g/255., b/255. };
}

constexpr color_argb make_color_argb_argb888(float_type a, uint8 r, uint8 g, uint8 b)
{
	return color_argb { a, r/255., g/255., b/255. };
}

constexpr color_argb make_color_argb_argb8888(uint8 a, uint8 r, uint8 g, uint8 b)
{
	return color_argb { a/255., r/255., g/255., b/255. };
}

constexpr color_argb make_color_argb_cmyk(float_type c, float_type m, float_type y, float_type k)
{
	return color_argb { 1., (1 - c) * (1 - k), (1 - m) * (1 - k), (1 - y) * (1 - k) };
}

constexpr color_argb make_color_argb_acmyk(float_type a, float_type c, float_type m, float_type y, float_type k)
{
	return color_argb { a, (1 - c) * (1 - k), (1 - m) * (1 - k), (1 - y) * (1 - k) };
}

constexpr bool operator!=(color_argb const& a, color_argb const& b)
{
	return a.a != b.a || a.r != b.r || a.g != b.g || a.b != b.b;
}

constexpr bool operator==(color_argb const& a, color_argb const& b)
{
	return a.a == b.a && a.r == b.r && a.g == b.g && a.b == b.b;
}

////////////////////////////////////////////////////////////////////////////////
constexpr color_argb k_color_white   { 1., 1., 1., 1. };
constexpr color_argb k_color_black   { 1., 0., 0., 0. };
constexpr color_argb k_color_clear   { 0., 0., 0., 0. };
constexpr color_argb k_color_red     { 1., 1., 0., 0. };
constexpr color_argb k_color_green   { 1., 0., 1., 0. };
constexpr color_argb k_color_blue    { 1., 0., 0., 1. };
constexpr color_argb k_color_cyan    { 1., 0., 1., 1. };
constexpr color_argb k_color_magenta { 1., 1., 0., 1. };
constexpr color_argb k_color_yellow  { 1., 1., 1., 0. };

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_COLOR__HPP_ */
