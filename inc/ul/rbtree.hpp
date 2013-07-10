//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//==============================================================================

#ifndef UL_RBTREE__HPP_
#define UL_RBTREE__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/rbtree_node.hpp>
#include <ul/rbtree_iterator.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <iterator>
#include <algorithm>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T, rbtree_node T::* NodeMember, class Compare = std::less<T> >
class rbtree {
	rbtree(const rbtree&);
	rbtree& operator=(const rbtree&);

public:
	typedef T*                                    pointer;
	typedef T const*                              const_pointer;
	typedef T&                                    reference;
	typedef T const&                              const_reference;
	typedef rbtree_iterator<T, NodeMember>        iterator;
	typedef rbtree_iterator<T const, NodeMember>  const_iterator;
	typedef std::reverse_iterator<iterator>       reverse_iterator;
	typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

	rbtree()
		: _root(nullptr)
	{ }

	iterator insert_equal(reference elem)
	{
		rbtree_node*  node   = member_of(&elem, NodeMember);
		rbtree_node*  parent = nullptr;
		rbtree_node** next   = &_root;
		Compare       cmp;

		while (*next) {
			parent = *next;
			if (cmp(elem, *parent_of(*next, NodeMember)))
				next = &(*next)->left;
			else
				next = &(*next)->right;
		}
		*next = node;
		node->insert(&_root, parent);

		return iterator(node);
	}

	std::pair<iterator, bool> insert_unique(reference elem)
	{
		rbtree_node*  node   = member_of(&elem, NodeMember);
		rbtree_node*  parent = nullptr;
		rbtree_node** next   = &_root;
		Compare       cmp;

		while (*next) {
			parent = *next;
			if (cmp(elem, *parent_of(*next, NodeMember)))
				next = &(*next)->left;
			else if (cmp(*parent_of(*next, NodeMember), elem))
				next = &(*next)->right;
			else
				return std::pair<iterator, bool>(iterator(*next), false);
		}
		*next = node;
		node->insert(&_root, parent);

		return std::pair<iterator, bool>(iterator(node), true);
	}

	template<class Key>
	iterator find(Key const& key)
	{
		rbtree_node* n = find_impl(key);
		if (!n)
			return end();

		return iterator(n);
	}

	template<class Key>
	const_iterator find(Key const& key) const
	{
		rbtree_node* n = find_impl(key);
		if (!n)
			return end();

		return const_iterator(n);
	}

	void remove(iterator i)
	{
		remove(*i);
	}

	void remove(reference elem)
	{
		member_of(&elem, NodeMember)->remove(&_root);
	}

	template<class Key>
	bool remove(Key const& key)
	{
		rbtree_node* n = find_impl(key);
		if (!n)
			return false;

		n->remove(&_root);
		return true;
	}

	bool empty() const
	{
		return !_root;
	}

	void swap(rbtree& other)
	{
		std::swap(_root, other._root);
	}


	iterator begin()
	{
		return _root ? iterator(_root->min()) : iterator();
	}

	iterator end()
	{
		return _root ? iterator(_root->max(), rbtree_out_of_range_tag()) : iterator();
	}

	const_iterator begin() const
	{
		return _root ? const_iterator(_root->min()) : const_iterator();
	}

	const_iterator end() const
	{
		return _root ? const_iterator(_root->max(), rbtree_out_of_range_tag()) : const_iterator();
	}

	reverse_iterator rbegin()
	{
		return _root ? reverse_iterator(iterator(_root->max())) : reverse_iterator();
	}

	reverse_iterator rend()
	{
		return _root ? reverse_iterator(iterator(_root->min(), rbtree_out_of_range_tag()))
		             : reverse_iterator();
	}

	const_reverse_iterator rbegin() const
	{
		return _root ? const_reverse_iterator(const_iterator(_root->max())) : const_reverse_iterator();
	}

	const_reverse_iterator rend() const
	{
		return _root ? const_reverse_iterator(const_iterator(_root->min(), rbtree_out_of_range_tag()))
		             : const_reverse_iterator();
	}

private:
	template<class Key>
	rbtree_node* find_impl(Key const& key) const
	{
		rbtree_node* next = _root;

		while (next) {
			if (key < *parent_of(next, NodeMember))
				next = next->left;
			else if (key > *parent_of(next, NodeMember))
				next = next->right;
			else
				return next;
		}

		return nullptr;
	}

private:
	rbtree_node* _root;
};

template<class T, rbtree_node T::* NodeMember>
inline void swap(rbtree<T, NodeMember>& rhs, rbtree<T, NodeMember>& lhs)
{
	rhs.swap(lhs);
}

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_RBTREE__HPP_ */
