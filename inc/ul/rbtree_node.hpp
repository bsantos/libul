//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_RBTREE_NODE__HPP_
#define UL_RBTREE_NODE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/debug.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
/**
 * \brief A raw reb-black binary tree node class to support custom
 *        implementations of algorithms based on balanced binary tree.
 */
struct rbtree_node {
	rbtree_node()
		: _parent(0), left(nullptr), right(nullptr)
	{ }


	void insert(rbtree_node** root, rbtree_node* parent);
	void remove(rbtree_node** root);

	rbtree_node* max() const;
	rbtree_node* min() const;
	rbtree_node* next() const;
	rbtree_node* prev() const;
	rbtree_node* root() const;


	enum color_type {
		red   = 0,
		black = 1
	};

	typedef size_t uintptr;

	static const uintptr k_color_mask = 0x3;

	void parent(rbtree_node* p)
	{
		_parent = reinterpret_cast<uintptr>(p) | _color;
	}
	rbtree_node* parent()
	{
		return reinterpret_cast<rbtree_node*>(_parent & ~k_color_mask);
	}

	void color(color_type c)
	{
		_color = c;
	}
	color_type const color() const
	{
		return static_cast<color_type>(_color);
	}

private:
	union {
		uintptr _parent;
		uint    _color : 2;
	};

public:
	rbtree_node* left;
	rbtree_node* right;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif	/* UL_RBTREE_NODE__HPP_ */
