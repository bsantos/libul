//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_LIST_NODE__HPP_
#define UL_LIST_NODE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
/**
 * \brief Defines a raw double linked list hook class to support custom
 *        implementations of algorithms based on list.
 */
struct list_node {
	list_node()
		: next(this), prev(this)
	{ }

	void push_front(list_node* node)
	{
		next->prev = node;
		node->prev = this;
		node->next = next;
		next = node;
	}

	void push_back(list_node* node)
	{
		prev->next = node;
		node->next = this;
		node->prev = prev;
		prev = node;
	}

	list_node* pop_front()
	{
		list_node* node = next;
		node->remove();
		return node;
	}

	list_node* pop_back()
	{
		list_node* node = prev;
		node->remove();
		return node;
	}

	list_node* front() const { return next; }
	list_node* back() const  { return prev; }

	void swap(list_node& y)
	{
		list_node* n = next;
		list_node* p = prev;

		next = y.next;
		prev = y.prev;
		y.next = n;
		y.prev = p;
	}

	void reverse()
	{
		list_node* n = next;

		next = prev;
		prev = n;
	}

	void remove()
	{
		prev->next = next;
		next->prev = prev;
	}

	bool empty() const { return (next == this); }


	list_node* next;
	list_node* prev;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_LIST_NODE__HPP_ */
