//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//=============================================================================

#ifndef UL_LIST_ITERATOR__HPP_
#define UL_LIST_ITERATOR__HPP_

///////////////////////////////////////////////////////////////////////////////
#include <ul/base.hpp>
#include <ul/list_node.hpp>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/mpl/if.hpp>

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
template<class T, list_node T::* NodeMember>
class list_iterator
	: public boost::iterator_facade<list_iterator<T, NodeMember>, T, boost::bidirectional_traversal_tag> {

	friend class boost::iterator_core_access;

	typedef typename boost::mpl::if_<boost::is_const<T>, list_node const, list_node>::type node_type;

public:
	list_iterator()
		: _node(nullptr)
	{ }
	explicit list_iterator(node_type* node)
		: _node(const_cast<list_node*>(node))
	{ }

private:
	void increment() { _node = _node->next; }
	void decrement() { _node = _node->prev; }

	bool equal(list_iterator const& other) const { return _node == other._node; }

	T& dereference() const { return *parent_of(_node, NodeMember); }

	list_node* _node;
};

///////////////////////////////////////////////////////////////////////////////
} /* namespace ul */

// EOF ////////////////////////////////////////////////////////////////////////
#endif /* UL_LIST_ITERATOR__HPP_ */
