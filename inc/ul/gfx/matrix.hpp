//=============================================================================
// Brief : GFX Affine Matrix
//-----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_MATRIX__HPP_
#define UL_GFX_MATRIX__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/gfx/types.hpp>
#include <cmath>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
struct affine_matrix {
	float_type a,  b;
	float_type c,  d;
	float_type tx, ty;

	constexpr affine_matrix()
		: a(), b(), c(), d(), tx(), ty()
	{ }

	constexpr affine_matrix(float_type a,  float_type b,
	                        float_type c,  float_type d,
	                        float_type tx, float_type ty)
		: a(a), b(b), c(c), d(d), tx(tx), ty(ty)
	{ }

	UL_GFX_AFFINE_MATRIX_CONVERTION_SUPPORT
};

constexpr affine_matrix k_affine_identity { 1, 0, 0, 1, 0, 0 };

struct affine_scale : affine_matrix {
	constexpr affine_scale(float_type sx, float_type sy)
		: affine_matrix(sx, 0, 0, sy, 0, 0)
	{ }
};

struct affine_rotate : affine_matrix {
	/*constexpr*/ affine_rotate(float_type rad)
		: affine_matrix(std::cos(rad), std::sin(rad), -std::sin(rad), std::cos(rad), 0, 0)
	{ }
};

struct affine_translate : affine_matrix {
	constexpr affine_translate(float_type tx, float_type ty)
		: affine_matrix(1, 0, 0, 1, tx, ty)
	{ }
};

affine_matrix const operator*(affine_matrix const& a, affine_matrix const& b);
affine_matrix const scale(affine_matrix const& a, float_type sx, float_type sy);
affine_matrix const rotate(affine_matrix const& a, float_type rad);
affine_matrix const translate(affine_matrix const& a, float_type tx, float_type ty);
affine_matrix const invert(affine_matrix const& a);

inline point const apply(point p, affine_matrix const& m)
{	//| a  b  0 | x | x' = a*x + c*y + tx
	//| c  d  0 | y | y' = b*x + d*y + ty
	//| tx ty 1 | 0 |
	return point { m.a * p.x + m.c * p.y + m.tx,
	               m.b * p.x + m.d * p.y + m.ty };
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_MATRIX__HPP_ */
