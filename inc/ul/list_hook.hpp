//=============================================================================
// Brief : Intrusive Double Linked List Hook
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2006-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_LIST_HOOK__HPP_
#define UL_LIST_HOOK__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
/**
 * \brief Defines a raw double linked list hook class to support custom
 *        implementations of algorithms based on list.
 */
struct list_hook {
	void init() { next = prev = this; }

	void push_front(list_hook* node)
	{
		next->prev = node;
		node->prev = this;
		node->next = next;
		next = node;
	}

	void push_back(list_hook* node)
	{
		prev->next = node;
		node->next = this;
		node->prev = prev;
		prev = node;
	}

	list_hook* pop_front()
	{
		list_hook* node = next;
		node->remove();
		return node;
	}

	list_hook* pop_back()
	{
		list_hook* node = prev;
		node->remove();
		return node;
	}

	list_hook* front() { return prev; }
	list_hook* back()  { return next; }

	void swap(list_hook& y)
	{
		list_hook* n = next;
		list_hook* p = prev;

		next = y.next;
		prev = y.prev;
		y.next = n;
		y.prev = p;
	}

	void reverse()
	{
		list_hook* n = next;

		next = prev;
		prev = n;
	}

	void remove()
	{
		prev->next = next;
		next->prev = prev;
	}

	bool empty() const { return (next == this); }


	list_hook* next;
	list_hook* prev;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_LIST_HOOK__HPP_ */
