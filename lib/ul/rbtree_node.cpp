//=============================================================================
// UL - Utilities Library
//
// Copyright (C) 2006-2013 Bruno Santos <bsantos@cppdev.net>
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

#include <ul/rbtree_node.hpp>

///////////////////////////////////////////////////////////////////////////////
static void rotate_left(ul::rbtree_node* node, ul::rbtree_node** root)
{
	ul::rbtree_node* tmp = node->right;

	if ((node->right = tmp->left))
		tmp->left->parent(node);
	tmp->left = node;

	tmp->parent(node->parent());
	if (tmp->parent()) {
		if (node == node->parent()->left) {
			node->parent()->left = tmp;
		} else {
			node->parent()->right = tmp;
		}
	} else {
		*root = tmp;
	}
	node->parent(tmp);
}

static void rotate_right(ul::rbtree_node* node, ul::rbtree_node** root)
{
	ul::rbtree_node* tmp = node->left;

	if ((node->left = tmp->right))
		tmp->right->parent(node);
	tmp->right = node;

	tmp->parent(node->parent());
	if (tmp->parent()) {
		if (node == node->parent()->right) {
			node->parent()->right = tmp;
		} else {
			node->parent()->left = tmp;
		}
	} else {
		*root = tmp;
	}
	node->parent(tmp);
}

static void remove_color(ul::rbtree_node* node, ul::rbtree_node* parent, ul::rbtree_node** root)
{
	ul::rbtree_node* sibling;

	//
	// Case 1
	//
	if (node && node->color() == ul::rbtree_node::red) {
		node->color(ul::rbtree_node::black);

	//
	// Cases: 2, 3, 4, 5, 6
	//
	// To reduce a number of repeated comparisions for the sibling, the code was duplicated and,
	// as a result, dead code paths were removed.
	//
	} else while (parent) {
		UL_ASSERT((node == parent->left) || (node == parent->right));
		if (node == parent->left) {
			sibling = parent->right;

			if (sibling->color() == ul::rbtree_node::red) {
				parent->color(ul::rbtree_node::red);
				sibling->color(ul::rbtree_node::black);
				rotate_left(parent, root);
				sibling = parent->right;
			}

			if (sibling->color() == ul::rbtree_node::black) {
				if (!sibling->right || sibling->right->color() == ul::rbtree_node::black) {
					if (!sibling->left || sibling->left->color() == ul::rbtree_node::black) {

						if (parent->color() == ul::rbtree_node::black) {
							sibling->color(ul::rbtree_node::red);
							node = parent;
							parent = node->parent();
							continue;

						} else {
							sibling->color(ul::rbtree_node::red);
							parent->color(ul::rbtree_node::black);
							break;
						}

					} else {
						sibling->color(ul::rbtree_node::red);
						sibling->left->color(ul::rbtree_node::black);
						rotate_right(sibling, root);
						sibling = parent->right;
					}
				}
				sibling->color(parent->color());
				parent->color(ul::rbtree_node::black);
				if (sibling->right)
					sibling->right->color(ul::rbtree_node::black);
				rotate_left(parent, root);
			}

		} else {
			sibling = parent->left;

			if (sibling->color() == ul::rbtree_node::red) {
				parent->color(ul::rbtree_node::red);
				sibling->color(ul::rbtree_node::black);
				rotate_right(parent, root);
				sibling = parent->left;
			}

			if (sibling->color() == ul::rbtree_node::black) {
				if (!sibling->left || sibling->left->color() == ul::rbtree_node::black) {
					if (!sibling->right || sibling->right->color() == ul::rbtree_node::black) {

						if (parent->color() == ul::rbtree_node::black) {
							sibling->color(ul::rbtree_node::red);
							node = parent;
							parent = node->parent();
							continue;

						} else {
							sibling->color(ul::rbtree_node::red);
							parent->color(ul::rbtree_node::black);
							break;
						}

					} else {
						sibling->color(ul::rbtree_node::red);
						sibling->right->color(ul::rbtree_node::black);
						rotate_left(sibling, root);
						sibling = parent->left;
					}
				}
				sibling->color(parent->color());
				parent->color(ul::rbtree_node::black);
				if (sibling->left) sibling->left->color(ul::rbtree_node::black);
				rotate_right(parent, root);
			}
		}
		break;
	}
}

///////////////////////////////////////////////////////////////////////////////
namespace ul {

///////////////////////////////////////////////////////////////////////////////
void rbtree_node::insert(rbtree_node** root, rbtree_node* parent)
{
	rbtree_node* node = this;
	rbtree_node* uncle;
	rbtree_node* grandparent;

	_parent = reinterpret_cast<uintptr>(parent);
	_color = red;
	left = nullptr;
	right = nullptr;

	for (;;) {
		if (node->parent() == nullptr) {
			node->color(black);
			return;

		} else if (node->parent()->color() == black) {
			return;

		} else {
			grandparent = node->parent()->parent();
			if (node->parent() == grandparent->left) {
				uncle = grandparent->right;
			} else {
				uncle = grandparent->left;
			}

			if (uncle != nullptr && uncle->color() == red) {
				node->color(red);
				node->parent()->color(black);
				uncle->color(black);
				grandparent->color(red);
				node = grandparent;
				continue;

			} else {
				node->color(red);
				if (node == node->parent()->right && node->parent() == grandparent->left) {
					rotate_left(node->parent(), root);
					node = node->left;

				} else if (node == node->parent()->left && node->parent() == grandparent->right) {
					rotate_right(node->parent(), root);
					node = node->right;
				}

				node->parent()->color(black);
				grandparent->color(red);

				if (node == node->parent()->left && node->parent() == grandparent->left) {
					rotate_right(grandparent, root);
				} else {
					rotate_left(grandparent, root);
				}
			}
		}
		break;
	}
}

void rbtree_node::remove(rbtree_node** root)
{
	rbtree_node* node = this;
	rbtree_node* child;
	rbtree_node* parent;
	int color;

	//
	// Satisfy Precondition: node has at most one non-null child
	//
	if (!node->left) {
		child = node->right;

	} else if (!node->right) {
		child = node->left;

	} else {
		rbtree_node* old = node;

		//
		// Locate a node that satisfies the precondition to swap with the node to be removed
		//
		node = node->right;
		while (node->left)
			node = node->left;

		//
		// Node satisfies precondition
		//
		child = node->right;

		//
		// Swap nodes (swap <-> (node <-> child))
		//
		parent = node->parent();
		color = node->color();

		if (child)
			child->parent(parent);

		if (parent == old) {
			parent->right = child;
			parent = node;
		} else {
			parent->left = child;
		}

		node->_parent = old->_parent;
		node->left = old->left;
		node->right = old->right;

		if (old->parent()) {
			UL_ASSERT((old->parent()->right == old) || (old->parent()->left == old));

			if (old->parent()->left == old) {
				old->parent()->left = node;

			} else {
				old->parent()->right = node;
			}

		} else {
			*root = node;
		}

		old->left->parent(node);
		if (old->right)
			old->right->parent(node);

		goto rm_color;
	}

	//
	// Replace Node with Child
	//
	parent = node->parent();
	color = node->color();
	if (child) {
		child->parent(parent);
	}
	if (parent) {
		UL_ASSERT((parent->right == node) || (parent->left == node));

		if (parent->left == node) {
			parent->left = child;

		} else {
			parent->right = child;
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

	UL_ASSERT(!*root || (*root)->color() == black);

	//
	// Bug prevention
	//
	this->_parent = 0;
	this->left = nullptr;
	this->right = nullptr;
}

rbtree_node* rbtree_node::max() const
{
	rbtree_node* root = const_cast<rbtree_node*>(this);

	while (root->right) {
		root = root->right;
	}
	return root;
}

rbtree_node* rbtree_node::min() const
{
	rbtree_node* root = const_cast<rbtree_node*>(this);

	while (root->left) {
		root = root->left;
	}
	return root;
}

rbtree_node* rbtree_node::prev() const
{
	rbtree_node* root = const_cast<rbtree_node*>(this);

	if (root->left) {
		root = root->left;
		while (root->right)
			root = root->right;
		return root;
	}

	while (root->parent() && root == root->parent()->left)
		root = root->parent();

	return root->parent();
}

rbtree_node* rbtree_node::next() const
{
	rbtree_node* root = const_cast<rbtree_node*>(this);

	if (root->right) {
		root = root->right;
		while (root->left) {
			root = root->left;
		}
		return root;
	}

	while (root->parent() && root == root->parent()->right)
		root = root->parent();

	return root->parent();
}

rbtree_node* rbtree_node::root() const
{
	rbtree_node* root = const_cast<rbtree_node*>(this);
	rbtree_node* next;

	while ((next = root->parent()))
		root = next;

	return root;
}

#if 0
void rbtree_node::dump(std::ostream& out, unsigned maxDeep)
{
	rbtree_node* node = this;
	unsigned deep = 0;

	//
	// maxDeep is how far we look dawn into the tree
	//
	while (node->right && deep < maxDeep) {
		node = node->right;
		++deep;
	}

	while (node) {
		out << std::string(' ', deep * 3) << "-"  << ((node->color() == red) ? 'R' : 'B') << " \n";

		if (node->left) {
			node = node->left;
			++deep;
			while (node->right && deep < maxDeep) {
				node = node->right;
				++deep;
			}
		} else {
			while (node->parent() && node == node->parent()->left) {
				node = node->parent();
				deep--;
			}
			node = node->parent();
			deep--;
		}
	}
}
#endif

///////////////////////////////////////////////////////////////////////////////
} /* namspace ul */

// EOF ////////////////////////////////////////////////////////////////////////
