//=============================================================================
// Brief : GFX Types - float, point, rect
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_TYPES__HPP_
#define UL_GFX_TYPES__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>

#ifdef __APPLE__
#	include <ul/gfx/types_osx.hpp>
#endif

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
static constexpr float_type k_ppi  = 1.0/72;
static constexpr float_type k_ppcm = 2.54/72;

////////////////////////////////////////////////////////////////////////////////
struct alignas(16) point {
	float_type x;
	float_type y;

	constexpr point()
		: x(), y()
	{ }

	constexpr point(float_type x, float_type y)
		: x(x), y(y)
	{ }

	UL_GFX_POINT_CONVERTION_SUPPORT
};

inline point const operator+(point a, point b)
{
	return { a.x + b.x, a.y + b.y };
}

inline point const operator-(point a, point b)
{
	return { a.x + b.x, a.y + b.y };
}

////////////////////////////////////////////////////////////////////////////////
struct rect {
	point origin;
	point size;

	constexpr rect()
		: origin(), size()
	{ }

	constexpr rect(float_type x, float_type y, float_type width, float_type height)
		: origin(x, y), size(width, height)
	{ }

	constexpr rect(point origin, point size)
		: origin(origin), size(size)
	{ }

	UL_GFX_RECT_CONVERTION_SUPPORT

	float_type x1() const { return { origin.x }; }
	float_type y1() const { return { origin.y }; }
	float_type x2() const { return { origin.x + size.x }; }
	float_type y2() const { return { origin.y + size.y }; }

	point p1() const { return { x1(), y1() }; }
	point p2() const { return { x2(), y2() }; }

	bool contains(point p) const
	{
		return p.x >= x1() && p.x <= x2()
		    && p.y >= y1() && p.y <= y2();
	}

	bool contains(rect const& r) const
	{
		return r.x1() >= x1() && r.x2() <= x2()
		    && r.y1() >= y1() && r.y2() <= y2();
	}

	bool intersects(rect const& r)
	{
		return r.x1() < x2() && y1() < r.y2() && r.y1() < y2();
	}
};

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_TYPES__HPP_ */
