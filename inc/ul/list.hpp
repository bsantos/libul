//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_LIST__HPP_
#define UL_LIST__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/list_node.hpp>
#include <ul/list_iterator.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
/**
 * \brief Linked list intrusive container that offers an interface similar to
 *        STL containers.
 */
template<class T, list_node T::* NodeMember>
class list {
	list(const list&);
	list& operator=(const list&);

public:
	typedef T*                                    pointer;
	typedef T const*                              const_pointer;
	typedef T&                                    reference;
	typedef T const&                              const_reference;
	typedef list_iterator<T, NodeMember>          iterator;
	typedef list_iterator<T const, NodeMember>    const_iterator;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

public:
	list()
	{ }
	~list()
	{ _root.remove(); }


	void push_front(reference node) { _root.push_front(member_of(&node, NodeMember)); }
	void push_back(reference node)  { _root.push_back(member_of(&node, NodeMember)); }

	pointer pop_front() { return parent_of(_root.pop_front(), NodeMember); }
	pointer pop_back()  { return parent_of(_root.pop_back(), NodeMember); }

	void remove(iterator i)  { remove(*i); }
	void remove(reference r) { member_of(&r, NodeMember)->remove(); }

	reference front() { return *parent_of(_root.front(), NodeMember); }
	reference back() { return *parent_of(_root.back(), NodeMember); }

	const_reference front() const { return *parent_of(_root.front(), NodeMember); }
	const_reference back() const  { return *parent_of(_root.back(), NodeMember); }

	bool empty() const { return _root.empty(); }

	iterator begin() { return iterator(_root.next); }
	iterator end()   { return iterator(&_root); }

	const_iterator begin() const { return const_iterator(_root.next); }
	const_iterator end() const   { return const_iterator(&_root); }

	reverse_iterator rbegin() { return reverse_iterator(iterator(_root.prev)); }
	reverse_iterator rend()   { return reverse_iterator(iterator(&_root)); }

	const_reverse_iterator rbegin() const { return const_reverse_iterator(const_iterator(_root.prev)); }
	const_reverse_iterator rend() const   { return const_reverse_iterator(const_iterator(&_root)); }

	void reverse() { _root.reverse(); }

	void swap(list& l) { _root.swap(l._root); }

private:
	list_node _root;
};

template<class T, list_node T::* NodeMember>
inline void swap(list<T, NodeMember>& rhs, list<T, NodeMember>& lhs)
{
	rhs.swap(lhs);
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_LIST__HPP_ */
