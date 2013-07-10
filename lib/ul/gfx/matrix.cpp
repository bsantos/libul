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

#include <ul/gfx/matrix.hpp>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
affine_matrix const operator*(affine_matrix const& a, affine_matrix const& b)
{	//| aa ab 0 | | ba bb 0 | | aa*ba+ab*bc    aa*bb+ab*bd    0 |
	//| ac ad 0 |*| bc bd 0 |=| ac*ba+ad*bc    ac*bb+ad*bd    0 |
	//| ax ay 1 | | bx by 1 | | ax*ba+ay*bc+bx ax*bb+ay*bd+by 1 |
	return affine_matrix { a.a  * b.a + a.b  * b.c,        a.a  * b.b + a.b  * b.d,
	                       a.c  * b.a + a.d  * b.c,        a.c  * b.b + a.d  * b.d,
	                       a.tx * b.a + a.ty * b.c + b.tx, a.tx * b.b + a.ty * b.d + b.ty };
}

affine_matrix const scale(affine_matrix const& a, float_type sx, float_type sy)
{
	return affine_matrix { a.a  * sx, a.b  * sy,
	                       a.c  * sx, a.d  * sy,
	                       a.tx * sx, a.ty * sy };
}

affine_matrix const rotate(affine_matrix const& a, float_type rad)
{
	float_type const c = std::cos(rad);
	float_type const s = std::sin(rad);

	return affine_matrix { a.a  * c + a.b  * -s, a.a  * s + a.b  * c,
	                       a.c  * c + a.d  * -s, a.c  * s + a.d  * c,
	                       a.tx * c + a.ty * -s, a.tx * s + a.ty * c };
}

affine_matrix const translate(affine_matrix const& a, float_type tx, float_type ty)
{
	return affine_matrix { a.a,       a.b,
	                       a.c,       a.d,
	                       a.tx + tx, a.ty + ty };
}

affine_matrix const invert(affine_matrix const& a)
{	//| a b 0 |-1 |  d        -b        0         |
	//| c d 0 |  =| -c         a        0         |
	//| x y 1 |   | (c*y-d*x) (b*x-a*y) (a*d-b*c) |
	float_type const m33 = a.a * a.d - a.b * a.c;
	if (UL_UNLIKELY(!m33))
		return a;

	return affine_matrix {  a.d/m33,                      -a.b/m33,
	                       -a.c/m33,                       a.a/m33,
	                       (a.c * a.ty - a.d * a.tx)/m33, (a.b * a.tx - a.a * a.ty)/m33 };
}

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
