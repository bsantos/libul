//=============================================================================
// Brief     : Red Black Tree - Self Balanced Binary Tree Support
// ----------------------------------------------------------------------------
// UL - Utilities Library
//
// Copyright (C) 2006-2010 Bruno Santos <bsantos@av.it.pt>
//
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)
//
//-----------------------------------------------------------------------------
// Implements the necessary code to balance a red-black tree.
//
// Red-Black Tree properties:
//   1- a node is red or black
//   2- root is always black
//   3- all leaves (null) nodes are black
//   4- both children of every red node are black
//   5- All paths from any given node to its leaves nodes
//      contains the same number of black nodes
//
//   http://en.wikipedia.org/wiki/Red-black_tree
//=============================================================================

#include <ul/rbtree_hook.hpp>

///////////////////////////////////////////////////////////////////////////////
static void rotate_left(ul::rbtree_hook* node, ul::rbtree_hook** root)
{
	ul::rbtree_hook* tmp = node->_right;

	if ((node->_right = tmp->_left))
		tmp->_left->set_parent(node);
	tmp->_left = node;

	tmp->set_parent(node->get_parent());
	if (tmp->get_parent()) {
		if (node == node->get_parent()->_left) {
			node->get_parent()->_left = tmp;
		} else {
			node->get_parent()->_right = tmp;
		}
	} else {
		*root = tmp;
	}
	node->set_parent(tmp);
}

static void rotate_right(ul::rbtree_hook* node, ul::rbtree_hook** root)
{
	ul::rbtree_hook* tmp = node->_left;

	if ((node->_left = tmp->_right))
		tmp->_right->set_parent(node);
	tmp->_right = node;

	tmp->set_parent(node->get_parent());
	if (tmp->get_parent()) {
		if (node == node->get_parent()->_right) {
			node->get_parent()->_right = tmp;
		} else {
			node->get_parent()->_left = tmp;
		}
	} else {
		*root = tmp;
	}
	node->set_parent(tmp);
}

static void remove_color(ul::rbtree_hook* node, ul::rbtree_hook* parent, ul::rbtree_hook** root)
{
	ul::rbtree_hook* sibling;

	//
	// Case 1
	//
	if (node && node->_color == ul::rbtree_hook::red) {
		node->_color = ul::rbtree_hook::black;

	//
	// Cases: 2, 3, 4, 5, 6
	//
	// To reduce a number of repeated comparisions for the sibling, the code was duplicated and,
	// as a result, dead code paths were removed.
	//
	} else while (parent) {
		UL_ASSERT((node == parent->_left) || (node == parent->_right));
		if (node == parent->_left) {
			sibling = parent->_right;

			if (sibling->_color == ul::rbtree_hook::red) {
				parent->_color = ul::rbtree_hook::red;
				sibling->_color = ul::rbtree_hook::black;
				rotate_left(parent, root);
				sibling = parent->_right;
			}

			if (sibling->_color == ul::rbtree_hook::black) {
				if (!sibling->_right || sibling->_right->_color == ul::rbtree_hook::black) {
					if (!sibling->_left || sibling->_left->_color == ul::rbtree_hook::black) {

						if (parent->_color == ul::rbtree_hook::black) {
							sibling->_color = ul::rbtree_hook::red;
							node = parent;
							parent = node->get_parent();
							continue;

						} else {
							sibling->_color = ul::rbtree_hook::red;
							parent->_color = ul::rbtree_hook::black;
							break;
						}

					} else {
						sibling->_color = ul::rbtree_hook::red;
						sibling->_left->_color = ul::rbtree_hook::black;
						rotate_right(sibling, root);
						sibling = parent->_right;
					}
				}
				sibling->_color = parent->_color;
				parent->_color = ul::rbtree_hook::black;
				if (sibling->_right)
					sibling->_right->_color = ul::rbtree_hook::black;
				rotate_left(parent, root);
			}

		} else {
			sibling = parent->_left;

			if (sibling->_color == ul::rbtree_hook::red) {
				parent->_color = ul::rbtree_hook::red;
				sibling->_color = ul::rbtree_hook::black;
				rotate_right(parent, root);
				sibling = parent->_left;
			}

			if (sibling->_color == ul::rbtree_hook::black) {
				if (!sibling->_left || sibling->_left->_color == ul::rbtree_hook::black) {
					if (!sibling->_right || sibling->_right->_color == ul::rbtree_hook::black) {

						if (parent->_color == ul::rbtree_hook::black) {
							sibling->_color = ul::rbtree_hook::red;
							node = parent;
							parent = node->get_parent();
							continue;

						} else {
							sibling->_color = ul::rbtree_hook::red;
							parent->_color = ul::rbtree_hook::black;
							break;
						}

					} else {
						sibling->_color = ul::rbtree_hook::red;
						sibling->_right->_color = ul::rbtree_hook::black;
						rotate_left(sibling, root);
						sibling = parent->_left;
					}
				}
				sibling->_color = parent->_color;
				parent->_color = ul::rbtree_hook::black;
				if (sibling->_left) sibling->_left->_color = ul::rbtree_hook::black;
				rotate_right(parent, root);
			}
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
void rbtree_hook::insert(rbtree_hook** root, rbtree_hook* parent)
{
	rbtree_hook* node = this;
	rbtree_hook* uncle;
	rbtree_hook* grandparent;

	_parent = parent;
	_color = red;
	_left = nullptr;
	_right = nullptr;

	for (;;) {
		if (node->get_parent() == nullptr) {
			node->_color = black;
			return;

		} else if (node->get_parent()->_color == black) {
			return;

		} else {
			grandparent = node->get_parent()->get_parent();
			if (node->get_parent() == grandparent->_left) {
				uncle = grandparent->_right;
			} else {
				uncle = grandparent->_left;
			}

			if (uncle != nullptr && uncle->_color == red) {
				node->_color = red;
				node->get_parent()->_color = black;
				uncle->_color = black;
				grandparent->_color = red;
				node = grandparent;
				continue;

			} else {
				node->_color = red;
				if (node == node->get_parent()->_right && node->get_parent() == grandparent->_left) {
					rotate_left(node->get_parent(), root);
					node = node->_left;

				} else if (node == node->get_parent()->_left && node->get_parent() == grandparent->_right) {
					rotate_right(node->get_parent(), root);
					node = node->_right;
				}

				node->get_parent()->_color = black;
				grandparent->_color = red;

				if (node == node->get_parent()->_left && node->get_parent() == grandparent->_left) {
					rotate_right(grandparent, root);
				} else {
					rotate_left(grandparent, root);
				}
			}
		}
		break;
	}
}

void rbtree_hook::remove(rbtree_hook** root)
{
	rbtree_hook* node = this;
	rbtree_hook* child;
	rbtree_hook* parent;
	int color;

	//
	// Satisfy Precondition: node has at most one non-null child
	//
	if (!node->_left) {
		child = node->_right;

	} else if (!node->_right) {
		child = node->_left;

	} else {
		rbtree_hook* old = node;

		//
		// Locate a node that satisfies the precondition to swap with the node to be removed
		//
		node = node->_right;
		while (node->_left)
			node = node->_left;

		//
		// Node satisfies precondition
		//
		child = node->_right;

		//
		// Swap nodes (swap <-> (node <-> child))
		//
		parent = node->get_parent();
		color = node->_color;

		if (child)
			child->set_parent(parent);

		if (parent == old) {
			parent->_right = child;
			parent = node;
		} else {
			parent->_left = child;
		}

		node->_parent = old->_parent;
		node->_left = old->_left;
		node->_right = old->_right;

		if (old->get_parent()) {
			UL_ASSERT((old->get_parent()->_right == old) || (old->get_parent()->_left == old));

			if (old->get_parent()->_left == old) {
				old->get_parent()->_left = node;

			} else {
				old->get_parent()->_right = node;
			}

		} else {
			*root = node;
		}

		old->_left->set_parent(node);
		if (old->_right)
			old->_right->set_parent(node);

		goto rm_color;
	}

	//
	// Replace Node with Child
	//
	parent = node->get_parent();
	color = node->_color;
	if (child) {
		child->set_parent(parent);
	}
	if (parent) {
		UL_ASSERT((parent->_right == node) || (parent->_left == node));

		if (parent->_left == node) {
			parent->_left = child;

		} else {
			parent->_right = child;
		}

	} else {
		*root = child;
	}

rm_color:
	//
	// Adjust Red-Black tree properties if node is black
	//
	if (color == black) {
		remove_color(child, parent, root);
	}

	//
	// Sanity check's
	//
	UL_ASSERT(!*root || (*root)->_color == black);

#ifndef NDEBUG
	this->_parent = nullptr;
	this->_left = nullptr;
	this->_right = nullptr;
#endif /* NDEBUG */
}

rbtree_hook* rbtree_hook::max()
{
	rbtree_hook* root = this;

	while (root->_right) {
		root = root->_right;
	}
	return root;
}

rbtree_hook* rbtree_hook::min()
{
	rbtree_hook* root = this;

	while (root->_left) {
		root = root->_left;
	}
	return root;
}

rbtree_hook* rbtree_hook::prev()
{
	rbtree_hook* root = this;

	if (root->_left) {
		root = root->_left;
		while (root->_right)
			root = root->_right;
		return root;
	}

	while (root->get_parent() && root == root->get_parent()->_left)
		root = root->get_parent();

	return root->get_parent();
}

rbtree_hook* rbtree_hook::next()
{
	rbtree_hook* root = this;

	if (root->_right) {
		root = root->_right;
		while (root->_left) {
			root = root->_left;
		}
		return root;
	}

	while (root->get_parent() && root == root->get_parent()->_right)
		root = root->get_parent();

	return root->get_parent();
}

void rbtree_hook::dump(std::ostream& out, unsigned maxDeep)
{
	rbtree_hook* node = this;
	unsigned deep = 0;

	//
	// maxDeep is how far we look dawn into the tree
	//
	while (node->_right && deep < maxDeep) {
		node = node->_right;
		++deep;
	}

	while (node) {
		out << std::string(' ', deep * 3) << "-"  << ((node->_color == red) ? 'R' : 'B') << " \n";

		if (node->_left) {
			node = node->_left;
			++deep;
			while (node->_right && deep < maxDeep) {
				node = node->_right;
				++deep;
			}
		} else {
			while (node->get_parent() && node == node->get_parent()->_left) {
				node = node->get_parent();
				deep--;
			}
			node = node->get_parent();
			deep--;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
} /* namspace ul */

// EOF ////////////////////////////////////////////////////////////////////////
