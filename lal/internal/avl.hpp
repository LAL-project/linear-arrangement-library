/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/
 
#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <cinttypes>
#include <vector>
#include <cmath>

namespace lal {
namespace internal {

template<class T>
class AVL {
public:
	~AVL() {
		free_node(root);
		root = nullptr;
	}

	[[nodiscard]]
	uint64_t remove(const T& x) noexcept {
		uint64_t top = 0;
		root = remove(root, x, top);
		return top;
	}

	// pre: -> v is sorted.
	//      -> v[0] > largest element of tree
	void join_sorted_all_greater(const std::vector<T>& v) noexcept {
		// do nothing if there is no data, duh
		if (v.size() == 0) { return; }
		// make the tree with the new info
		tree_node *n =
		_make_tree(
			v, 0, static_cast<int64_t>(v.size() - 1), nullptr, '0'
		);

		// join the two trees

		// if our root is empty then the new
		// node is the root of the new tree
		if (root == nullptr) {
			root = n;
			return;
		}

		// easy, degenerate cases
		if (root->tree_size == 1) {
			tree_node *r = insert(nullptr, n, '0', root->key);
			free_node(root);
			root = r;
		}
		else if (n->tree_size == 1) {
			tree_node *r = insert(nullptr, root, '0', n->key);
			free_node(n);
			root = r;
		}
		else {
			// complicated case
			root = (root->height >= n->height ?
					join_taller(root, n) :
					join_shorter(root, n));
		}

		//sanity_check();
	}

private:
	// ---------------------------------------------------------- //
	// DEFINITIONS

	struct tree_node {
		// contents of the node
		T key;

		// side of this node:
		//		l: this node is a left subtree
		//		r: this node is a right subtree
		//		0: this node is the root.
		//         Eq. parent = nullptr
		char side = '0';

		// Amount of nodes in the rooted at this node.
		// Number of nodes in the left and right subtrees
		// plus this node.
		uint64_t tree_size = 0;
		// Maximum height of the left and right subtrees' height
		uint64_t height = 0;
		// balance factor of a node:
		// right subtree's height - left subtree's height
		int64_t bf = 0;

		// parent of this node
		tree_node *parent = nullptr;
		// left and right subtrees
		tree_node *left = nullptr;
		tree_node *right = nullptr;

		void compute_height() noexcept {

			const int64_t lh =
			(left != nullptr ? static_cast<int64_t>(left->height) : -1);

			const int64_t rh =
			(right != nullptr ? static_cast<int64_t>(right->height) : -1);

			height = static_cast<uint64_t>(std::max(lh, rh)) + 1;
			bf = rh - lh;
		}
		void compute_size() noexcept {
			const uint64_t ls = (left != nullptr ? left->tree_size : 0);
			const uint64_t rs = (right != nullptr ? right->tree_size : 0);
			tree_size = 1 + ls + rs;
		}
		void update() noexcept {
			compute_size();
			compute_height();
		}

		void link_parent_to(tree_node *n) {
			if (n == nullptr) { return; }
			if (parent != nullptr) {
				if (side == 'l') {
					parent->left = n;
				}
				else if (side == 'r') {
					parent->right = n;
				}
			}
			n->parent = parent;
			n->side = side;
		}

		[[nodiscard]]
		uint64_t left_size() const noexcept {
			return (left == nullptr ? 0 : left->tree_size);
		}
		[[nodiscard]]
		uint64_t right_size() const noexcept {
			return (right == nullptr ? 0 : right->tree_size);
		}
	};

private:
	// ---------------------------------------------------------- //
	// ATTRIBUTES

	// root of the tree
	tree_node *root = nullptr;

private:
	// ---------------------------------------------------------- //
	// FUNCTIONS

	void free_node(tree_node *n) noexcept {
		if (n == nullptr) {
			return;
		}
		free_node(n->left);
		free_node(n->right);
		delete n;
	}

	/* --------- */
	/* ROTATIONS */

	// rotations of nodes
	//     assume _n has a left subtree
	[[nodiscard]]
	tree_node *right_rotation(tree_node *_n) noexcept {
#if defined DEBUG
		assert(_n != nullptr);
#endif

		tree_node *A = _n;
		tree_node *P = A->parent;
		tree_node *B = A->left;

#if defined DEBUG
		assert(B != nullptr);
#endif

		// update A's parent
		//    (notice P might be null, however,
		//    it is null only when A->side != 'r' and 'l')
		if (A->side == 'r') {
			P->right = B;
		}
		else if (A->side == 'l') {
			P->left = B;
		}
		// parent of B is now parent of A
		B->parent = P;

		//     if A is the left child, then B is
		//     the left child, same for 'right'
		B->side = A->side;

		// update A's parent, and A's side
		A->parent = B;
		A->side = 'r';

		// update node E
		tree_node *E = B->right;
		A->left = E;
		if (E != nullptr) {
			E->side = 'l';
			E->parent = A;
		}
		B->right = A;

		// update nodes A ...
		A->update();
		// ... and B
		B->update();
		return B;
	}
	// rotations of nodes
	//     assume _n has a right subtree
	[[nodiscard]]
	tree_node *left_rotation(tree_node *_n) noexcept {
#if defined DEBUG
		assert(_n != nullptr);
#endif

		tree_node *B = _n;
		tree_node *A = B->right;

		// parent of B is now parent of A
		A->parent = B->parent;
		//    update A's parent
		if (B->side == 'r') {
			B->parent->right = A;
		}
		else if (B->side == 'l') {
			B->parent->left = A;
		}
		//     if A is the left child, then B is
		//     the left child, ...
		A->side = B->side;

		// update A's parent, and A's side
		B->parent = A;
		B->side = 'l';

		// update node E
		tree_node *E = A->left;
		B->right = E;
		if (E != nullptr) {
			E->side = 'r';
			E->parent = B;
		}
		A->left = B;

		// update nodes B ...
		B->update();
		// ... and A
		A->update();
		return A;
	}
	// cases of imbalances:
	//     left_* cases assume that _n has left subtree
	//     right_* cases assume that _n has right subtree
	[[nodiscard]]
	tree_node *left_left_case(tree_node *n) noexcept {
		return right_rotation(n);
	}
	[[nodiscard]]
	tree_node *left_right_case(tree_node *n) noexcept {
		n->left = left_rotation(n->left);
		return right_rotation(n);
	}
	[[nodiscard]]
	tree_node *right_right_case(tree_node *n) noexcept {
		return left_rotation(n);
	}
	[[nodiscard]]
	tree_node *right_left_case(tree_node *n) noexcept {
		n->right = right_rotation(n->right);
		return left_rotation(n);
	}
	// returns the root of the new balanced tree
	[[nodiscard]]
	tree_node *balance(tree_node *n) noexcept {
		if (n == nullptr) { return nullptr; }
#if defined DEBUG
		assert(std::abs(n->bf) <= 2);
#endif

		if (std::abs(n->bf) <= 1) { return n; }
		return (
		n->bf == -2 ?
			(n->left->bf <= 0 ? left_left_case(n) : left_right_case(n)) :
			(n->right->bf >= 0 ? right_right_case(n) : right_left_case(n))
		);

		/*
		if (n->bf == -2) {
			// //if (n->left == nullptr) { return n; }


			if (n->left->bf <= 0) {
				return left_left_case(n);
			}
			return left_right_case(n);
		}

		//if (n->bf == +2) {
			//if (n->right == nullptr) { return n; }

			if (n->right->bf >= 0) {
				return right_right_case(n);
			}
			return right_left_case(n);
		//}
		//return n;
		*/
	}

	/* --------------------- */
	/* INSERTION OF ELEMENTS */

	/* p: Parent of n.
	 * n: Reference to node.
	 * s: paren't side (0: root, l: left, r: right)
	 * x: Element to be added.
	 *
	 * Returns the newly created tree node
	 */
	[[nodiscard]]
	tree_node *insert(tree_node *p, tree_node *n, char s, const T& x) noexcept {
		// create a new node
		if (n == nullptr) {
			n = new tree_node();

			n->key = x;

			n->parent = p;
			n->left = nullptr;
			n->right = nullptr;

			n->side = s;

			n->tree_size = 1;
			n->height = 0;
			n->bf = 0;
			return n;
		}

		if (x == n->key) {
			// do not insert already
			// existing values
			return n;
		}

		// insert as usual
		if (x < n->key) {
			n->left = insert(n, n->left, 'l', x);
		}
		else {
			n->right = insert(n, n->right, 'r', x);
		}

		// update node size and height
		n->update();
		return balance(n);
	}

	/* ------------------- */
	/* REMOVAL OF ELEMENTS */

	[[nodiscard]]
	tree_node *remove_leftmost(tree_node *n, T *k = nullptr) noexcept {
		if (n->left == nullptr) {
			// retrieve leftmost key
			if (k != nullptr) {
				*k = n->key;
			}

			tree_node *r = n->right;
			n->link_parent_to(r);

			delete n;
			return r;
		}
		n->left = remove_leftmost(n->left, k);
		n->update();
		return balance(n);
	}
	[[nodiscard]]
	tree_node *remove_rightmost(tree_node *n, T *k = nullptr) noexcept {
		if (n->right == nullptr) {
			// this is the rightmost
			if (k != nullptr) {
				*k = n->key;
			}

			tree_node *l = n->left;
			n->link_parent_to(l);

			delete n;
			return l;
		}

		n->right = remove_rightmost(n->right, k);
		n->update();
		return balance(n);
	}

	// In a "general" implementation of an AVL tree, after a recursive call
	// to 'remove', we may not want to continue doing work. Since this AVL
	// tree is going to be used in a way that all 'x' this function is called
	// with exist in the tree then we will always find this element in the
	// tree, so there will be work to be done after each recursive call to
	// 'remove'.
	[[nodiscard]]
	tree_node *remove(tree_node *n, const T& x, uint64_t& on_top) noexcept {
		if (n == nullptr) {
			// not found
			on_top = 0;
			return nullptr;
		}

		if (x < n->key) {
			on_top += n->right_size() + 1;
			n->left = remove(n->left, x, on_top);
			// update this node's size
			n->update();
			// balance 'n' to keep the AVL invariant
			return balance(n);
		}
		if (x > n->key) {
			n->right = remove(n->right, x, on_top);
			// update this node's size
			n->update();
			// balance 'n' to keep the AVL invariant
			return balance(n);
		}

		// found element at node 'n'

		// update amount of elements larger than 'x'
		on_top += n->right_size();

		// update tree
		tree_node *L = n->left;
		tree_node *R = n->right;
		if (L == nullptr and R == nullptr) {
			// nothing else to do, really
			delete n;
			return nullptr;
		}
		if (L != nullptr and R == nullptr) {
			n->link_parent_to(L);
			delete n;
			// L is already balanced: not need to do that
			return L;
		}
		if (L == nullptr and R != nullptr) {
			n->link_parent_to(R);
			delete n;
			// R is already balanced: not need to do that
			return R;
		}

		// L != nullptr and R != nullptr

		// find the smallest value in the right subtree,
		// or the largest in the left subtree,
		// depending on the height

		uint64_t dummy;
		if (L->height > R->height) {
			tree_node *lL = L;
			while (lL->right != nullptr) {
				lL = lL->right;
			}
			n->key = lL->key;
			n->left = remove(L, lL->key, dummy);
		}
		else {
			tree_node *sR = R;
			while (sR->left != nullptr) {
				sR = sR->left;
			}
			n->key = sR->key;
			n->right = remove(R, sR->key, dummy);
		}

		// copy the key into n
		n->update();
		return balance(n);
	}

	/* ----------------- */
	/* UNION OF TWO AVLS */

	// L := largest key in T1
	// S := smallest key in T2
	// pre:
	// -> height(T1) >= height(T2)
	// -> L < S
	[[nodiscard]]
	tree_node *join_taller(tree_node *T1, tree_node *T2) noexcept {
#if defined DEBUG
		assert(T1 != nullptr);
		assert(T2 != nullptr);
		assert(T1->tree_size > 1 and T2->tree_size > 1);
#endif

		// we need a new node anyway
		tree_node *x = new tree_node();
#if defined DEBUG
		assert(x != nullptr);
#endif

		// remove left-most element in T2
		T2 = remove_leftmost(T2, &(x->key));
		// find right-most node such that its height
		// is either (T2->height) or (T2->height + 1)
		// in T1
		const uint64_t h = T2->height;
		tree_node *v = T1;
		uint64_t hp = v->height;
		while (hp > h + 1 and v != nullptr) {
			hp = (v->bf == -1 ? hp - 2 : hp - 1);
			v = v->right;
		}

#if defined DEBUG
		assert(v != nullptr);
#endif

		// NOTE: 'u' is allowed to be nullptr!
		tree_node *u = v->parent;

		x->side = '0';
		x->parent = u;
		x->left = v;
		v->parent = x;
		v->side = 'l';
		x->right = T2;
		T2->side = 'r';
		T2->parent = x;
		x->update();

		// this is why 'u' is allowed to be nullptr!
		if (u == nullptr) {
			return balance(x);
		}

		u->right = x;
		x->side = 'r';
		x = balance(x);
		// climb up the tree rebalancing
		while (u->parent != nullptr) {
			u->update();
			u = balance(u);
			u = u->parent;
		}
#if defined DEBUG
		assert(u != nullptr);
#endif

		u->update();
		return balance(u);
	}
	// L := largest key in T1
	// S := smallest key in T2
	// pre:
	// -> height(T1) < height(T2)
	// -> L < S
	[[nodiscard]]
	tree_node *join_shorter(tree_node *T1, tree_node *T2) noexcept {
#if defined DEBUG
		assert(T1 != nullptr);
		assert(T2 != nullptr);
		assert(T1->tree_size > 1 and T2->tree_size > 1);
#endif

		// we need a new node anyway
		tree_node *x = new tree_node();
#if defined DEBUG
		assert(x != nullptr);
#endif

		// remove right-most element in T1
		T1 = remove_rightmost(T1, &(x->key));
		// find right-most node such that its height
		// is either (T1->height) or (T1->height + 1)
		// in T2
		const uint64_t h = T1->height;
		tree_node *v = T2;
		uint64_t hp = v->height;
		while (hp > h + 1 and v != nullptr) {
			hp = (v->bf == -1 ? hp - 2 : hp - 1);
			v = v->left;
		}
#if defined DEBUG
		assert(v != nullptr);
#endif

		// NOTE: 'u' is allowed to be nullptr!
		tree_node *u = v->parent;

		x->side = '0';
		x->parent = u;
		x->right = v;
		v->parent = x;
		v->side = 'r';
		x->left = T1;
		T1->side = 'l';
		T1->parent = x;
		x->update();

		// this is why 'u' is allowed to be nullptr!
		if (u == nullptr) {
			return balance(x);
		}
		u->left = x;
		x->side = 'l';
		x = balance(x);
		// climb up the tree rebalancing
		while (u->parent != nullptr) {
			u->update();
			u = balance(u);
			u = u->parent;
		}
#if defined DEBUG
		assert(u != nullptr);
#endif

		u->update();
		return balance(u);
	}

	/* ------ */
	/* OTHERS */

	[[nodiscard]]
	tree_node *_make_tree(
		const std::vector<T>& v,
		int64_t l, int64_t r, tree_node *p, char s
	)
	noexcept
	{
		if (l > r) { return nullptr; }
		const int64_t m = (l + r)/2;
#if defined DEBUG
		assert(m >= 0);
#endif

		// make a node with the element in the middle
		tree_node *n = new tree_node();
		n->key = v[static_cast<uint64_t>(m)];
		// make sure pointers are correct
		n->parent = p;
		n->side = s;
		// construct the tree making
		// sure the invariant is kept
		n->left = _make_tree(v, l, m - 1, n, 'l');
		n->right = _make_tree(v, m + 1, r, n, 'r');
		n->update();
		// by construction, there is no
		// need to balance the node 'n'
		return n;
	}
};

} // -- namespace internal
} // -- namespace lal
