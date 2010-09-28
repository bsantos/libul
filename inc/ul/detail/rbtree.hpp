//=============================================================================
// Brief : Red-Back Tree Intrusive Container Detail
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2006-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_DETAIL_RBTREE__HPP_
#define UL_DETAIL_RBTREE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/rbtree_hook.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T, rbtree_hook T::* NodeMember, class Compare>
class rbtree;

///////////////////////////////////////////////////////////////////////////////
namespace detail {

///////////////////////////////////////////////////////////////////////////////
template<class T, rbtree_hook T::* NodeMember, class Compare>
class rbtree_iterator : rbtree_hook_iterator {
	friend class rbtree<T, NodeMember, Compare>;

	rbtree_iterator(rbtree_hook* next, rbtree_hook* prev)
		: rbtree_hook_iterator(next, prev)
	{ }

public:
	rbtree_iterator()
	{ }

	T& operator*()
	{
		return *parent_of<T>(next(), NodeMember);
	}

	T* operator&()
	{
		return parent_of<T>(next(), NodeMember);
	}

	T* operator->()
	{
		return parent_of<T>(next(), NodeMember);
	}

	rbtree_iterator& operator++()
	{
		inc();
		return *this;
	}

	rbtree_iterator& operator--()
	{
		dec();
		return *this;
	}

	rbtree_iterator operator++(int)
	{
		rbtree_iterator tmp(*this);
		inc();
		return tmp;
	}

	rbtree_iterator operator--(int)
	{
		rbtree_iterator tmp(*this);
		dec();
		return tmp;
	}

	bool operator==(const rbtree_iterator& i) { return _next == i._next; }
	bool operator!=(const rbtree_iterator& i) { return _next != i._next; }
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace detail */ } /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_DETAIL_RBTREE__HPP_ */
