//=============================================================================
// Brief : Red Black Tree - Self Balanced Binary Tree Hook
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2006-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_RBTREE_HOOK__HPP_
#define UL_RBTREE_HOOK__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/debug.hpp>
#include <ostream>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
/**
 * \brief A raw reb-black binary tree node class to support custom
 *        implementations of algorithms based on balanced binary tree.
 */
struct rbtree_hook {
	void set_parent(rbtree_hook* p)
	{
		UL_ASSERT(!((size_t) p & 0x3));
		_parent = (rbtree_hook*) (((size_t) p) | ((size_t) _parent & 0x3));
	}

	rbtree_hook* get_parent()
	{
		return (rbtree_hook*) ((size_t) _parent & ~0x3);
	}

	enum {
		red   = 0,
		black = 1
	};

	union {
		rbtree_hook* _parent;
		int          _color : 2;
	};
	rbtree_hook*     _left;
	rbtree_hook*     _right;

	void init() { _parent = _left = _right = nullptr; }
	void insert(rbtree_hook** root, rbtree_hook* parent);
	void remove(rbtree_hook** root);

	rbtree_hook* max();
	rbtree_hook* min();
	rbtree_hook* next();
	rbtree_hook* prev();

	void dump(std::ostream& out, unsigned max_deep = 25);
};

///////////////////////////////////////////////////////////////////////////////
class rbtree_hook_iterator {
public:
	rbtree_hook_iterator()
		: _next(nullptr), _prev(nullptr)
	{ }
	rbtree_hook_iterator(rbtree_hook* next, rbtree_hook* prev)
		: _next(next), _prev(prev)
	{ }

//protected:
	void inc()
	{
		UL_ASSERT_PTR(_next);

		_prev = _next;
		_next = _next->next();
	}

	void dec()
	{
		UL_ASSERT_PTR(_prev);

		_next = _prev;
		_prev = _prev->prev();
	}

	rbtree_hook* next() { return _next; }
	rbtree_hook* prev() { return _prev; }

protected:
	rbtree_hook* _next;
	rbtree_hook* _prev;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif	/* UL_RBTREE_HOOK__HPP_ */
