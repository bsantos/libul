//=============================================================================
// Brief : GFX Types
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
#	error "Don't include this file, include types.hpp"
#endif

////////////////////////////////////////////////////////////////////////////////
#include <CoreGraphics/CGGeometry.h>
#include <CoreGraphics/CGAffineTransform.h>


typedef struct CGContext* CGContextRef;

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
typedef CGContextRef context_ref;
typedef CGFloat      float_type;

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#define UL_GFX_POINT_CONVERTION_SUPPORT \
	constexpr point(CGPoint const& p)   \
		: x(p.x), y(p.y)                \
	{ }                                 \
                                        \
	constexpr point(CGSize const& p)    \
		: x(p.width), y(p.height)       \
	{ }                                 \
	                                    \
	constexpr operator CGPoint() const  \
	{                                   \
		return CGPoint { x, y };        \
	}	                                \
	                                    \
	constexpr operator CGSize() const   \
	{                                   \
		return CGSize { x, y };         \
	}

#define UL_GFX_RECT_CONVERTION_SUPPORT                                      \
	constexpr rect(CGRect const& r)                                         \
		: origin(r.origin.x, r.origin.y), size(r.size.width, r.size.height) \
	{ }                                                                     \
                                                                            \
	constexpr operator CGRect() const                                       \
	{                                                                       \
		return CGRect { {origin.x, origin.y}, {size.x, size.y} };           \
	}

#define UL_GFX_AFFINE_MATRIX_CONVERTION_SUPPORT              \
	constexpr affine_matrix(CGAffineTransform const& m)      \
		: a(m.a), b(m.b), c(m.c), d(m.d), tx(m.tx), ty(m.ty) \
	{ }                                                      \
                                                             \
	constexpr operator CGAffineTransform() const             \
	{                                                        \
		return CGAffineTransform { a, b, c, d, tx, ty };     \
	}

////////////////////////////////////////////////////////////////////////////////
