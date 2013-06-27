//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_RBTREE_ITERATOR__HPP_
#define UL_RBTREE_ITERATOR__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/rbtree_node.hpp>
#include <boost/iterator/iterator_facade.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
struct rbtree_out_of_range_tag { };

template<class T, rbtree_node T::* NodeMember>
class rbtree_iterator
	: public boost::iterator_facade<rbtree_iterator<T, NodeMember>, T, boost::bidirectional_traversal_tag> {

	friend class boost::iterator_core_access;

	typedef size_t uintptr;

	static const uintptr k_out_of_range_bit = 0x1;

public:
	rbtree_iterator()
		: _node(nullptr)
	{ }
	explicit rbtree_iterator(rbtree_node* node)
		: _node(node)
	{ }
	rbtree_iterator(rbtree_node* node, rbtree_out_of_range_tag)
		: _node(node)
	{ _iptr |= k_out_of_range_bit; }

private:
	void increment()
	{
		rbtree_node* tmp = reinterpret_cast<rbtree_node*>(_iptr & ~k_out_of_range_bit);

		tmp = tmp->next();
		if (tmp)
			_node = tmp;
		else
			_iptr |= k_out_of_range_bit;
	}

	void decrement()
	{
		rbtree_node* tmp = reinterpret_cast<rbtree_node*>(_iptr & ~k_out_of_range_bit);

		tmp = tmp->next();
		if (tmp)
			_node = tmp;
		else
			_iptr |= k_out_of_range_bit;
	}

	bool equal(rbtree_iterator const& other) const { return _node == other._node; }

	T& dereference() const { return *parent_of(_node, NodeMember); }

	union {
		rbtree_node* _node;
		uintptr      _iptr;
	};
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif	/* UL_RBTREE_ITERATOR__HPP_ */
