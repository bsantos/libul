//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2013 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_GFX_TEXT__HPP_
#define UL_GFX_TEXT__HPP_

////////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/gfx/types.hpp>
#include <ul/gfx/textline.hpp>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
namespace ul { namespace gfx {

////////////////////////////////////////////////////////////////////////////////
class text {
public:
	text();
	text(text&&);
	text& operator=(text&&);
	~text();

private:
	std::vector<textline> _lines;
	rect                  _frame;
};

////////////////////////////////////////////////////////////////////////////////
} /* namespace gfx */ } /* namespace ul */

////////////////////////////////////////////////////////////////////////////////
#endif /* UL_GFX_TEXT__HPP_ */
