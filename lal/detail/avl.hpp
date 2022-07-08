/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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
#include <algorithm>
#include <cassert>
#endif
#include <cinttypes>
#include <vector>
#include <cmath>

// lal includes
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace detail {


/**
 * @brief Simple class that implements an AVL tree.
 *
 * This class can store repeated elements. However, it will never contain two
 * nodes with the same key. Every element is inserted with a total number of
 * occurrences associated to it. So an element inserted three times (and never
 * deleted) will be in the tree in a single node and its number of occurrences
 * will be 3. Each time an element is removed, its number of occurrences
 * decreases by 1.
 *
 * Deletion of nodes depends on the template parameter of functions @ref remove.
 * Read their documentation for further details.
 */
template <typename T>
class AVL {
public:
	/// Frequency of a value in the tree @e v.
	struct frequencies {
		/// Number of occurrences of @e v in the tree.
		std::size_t counter_equal;
		/// Number of occurrences of larger elements than @e v in the tree.
		std::size_t counter_larger;
		/// Number of nodes with a key larger than @e v in the tree.
		std::size_t num_nodes_larger;
		/// Equality comparison.
		bool operator== (const frequencies& f) const noexcept {
			return
				counter_equal == f.counter_equal and
				counter_larger == f.counter_larger and
				num_nodes_larger == f.num_nodes_larger;
		}
		/// Different operator.
		bool operator!= (const frequencies& f) const noexcept
		{ return not (*this == f); }
	};

public:
	/// Destructor.
	~AVL() noexcept {
		free_node(m_root);
		m_root = nullptr;
	}

	/**
	 * @brief Empties the tree.
	 *
	 * Clears all the memory used by this tree. References to its elements
	 * are invalidated.
	 */
	void clear() noexcept {
		free_node(m_root);
		m_root = nullptr;
	}

	/**
	 * @brief Finds the largest value.
	 * @returns A pair with a constant reference to the largest value and its
	 * frequency statistics.
	 */
	[[nodiscard]]
	std::pair<const T&, frequencies> get_largest_value() const noexcept {
#if defined DEBUG
		assert(m_root != nullptr);
#endif

		tree_node *n = m_root;
		while (n->right != nullptr) { n = n->right; }
		return {n->key, frequencies{n->node_counter,0,0}};
	}

	/**
	 * @brief Remove an element from the tree.
	 * @tparam use_counter If true then a node will be removed only if its
	 * counter has reached 0. If false then the node will be removed regardless
	 * of the value of the counter.
	 * @param v The element to be removed.
	 * @returns The element's frequency statistics before removal. If the node
	 * is removed from the tree the references returned by @ref get_largest_value
	 * are invalidated.
	 */
	template <bool use_counter>
	frequencies remove_largest() noexcept {
		frequencies freqs{0,0,0};
		m_root = remove_rightmost<use_counter>(m_root, nullptr, freqs);
		return freqs;
	}

	/**
	 * @brief Remove an element from the tree.
	 * @tparam use_counter If true then a node will be removed only if its
	 * counter has reached 0. If false then the node will be removed regardless
	 * of the value of the counter.
	 * @param v The element to be removed.
	 * @returns The element's frequency statistics before removal. If the node
	 * is removed from the tree the references returned by @ref get_largest_value
	 * are invalidated.
	 */
	template <bool use_counter>
	frequencies remove_smallest() noexcept {
		frequencies freqs{0,0,0};
		m_root = remove_leftmost<use_counter>(m_root, nullptr, freqs);
		return freqs;
	}

	/**
	 * @brief Find the largest value.
	 * @returns A pair with a constant reference to the largest value and its
	 * frequency statistics.
	 */
	[[nodiscard]]
	std::pair<const T&, frequencies> get_smallest_value() const noexcept {
#if defined DEBUG
		assert(m_root != nullptr);
#endif

		frequencies freqs{0,0,0};
		tree_node *n = m_root;
		while (n->left != nullptr) {
			freqs.counter_larger += n->node_counter + n->right_counter();
			freqs.num_nodes_larger += 1 + n->right_size();
			n = n->left;
		}
		freqs.counter_equal = n->node_counter;
		freqs.counter_larger += n->right_counter();
		freqs.num_nodes_larger += n->right_size();
		return {n->key, freqs};
	}

	/**
	 * @brief Number of occurrences associated to a given value.
	 * @param v Value to find.
	 * @returns The element's frequency statistics.
	 */
	[[nodiscard]] frequencies element_frequency(const T& v) const noexcept
	{
		frequencies res{0,0,0};
		tree_node *n = m_root;
		while (n != nullptr) {
			if (v == n->key) {
				res.counter_equal = n->node_counter;
				res.counter_larger += n->right_counter();
				res.num_nodes_larger += n->right_size();
				return res;
			}
			if (v < n->key) {
				res.counter_larger += n->node_counter + n->right_counter();
				res.num_nodes_larger += 1 + n->right_size();
				n = n->left;
			}
			else {
				n = n->right;
			}
		}
		return res;
	}

	/**
	 * @brief Insert a new value @e v into the tree.
	 * @param v New value.
	 * @returns The element's frequency statistics.
	 */
	template <typename U>
	[[nodiscard]] frequencies insert(U&& v) noexcept {
		frequencies freqs{0,0,0};
		m_root = insert(nullptr, m_root, std::forward<U>(v), freqs);
		return freqs;
	}

	/**
	 * @brief Remove an element from the tree.
	 * @tparam use_counter If true then a node will be removed only if its
	 * counter has reached 0. If false then the node will be removed regardless
	 * of the value of the counter.
	 * @param v The element to be removed.
	 * @returns The element's frequency statistics before removal.
	 */
	template <bool use_counter>
	[[nodiscard]] frequencies remove(const T& v) noexcept {
		frequencies freqs{0,0,0};
		m_root = remove<use_counter>(m_root, v, freqs);
		return freqs;
	}

	/**
	 * @brief Add to the tree the elements in the vector @e v.
	 *
	 * The element passed as parameter must be a std::vector<T>.
	 * @param v A sorted collection of unique elements.
	 * @pre The vector @e v is sorted.
	 * @pre The elements in vector @e v are unique.
	 * @pre The first element of the vector is larger than the largest element
	 * of the tree.
	 */
	template <typename U>
	void join_sorted_all_greater(U&& v) noexcept {
		using elem_t = typename std::remove_reference_t<U>::value_type;
		static constexpr bool rvalue = std::is_same_v<std::vector<elem_t>, U>;

#if defined DEBUG
		assert(std::is_sorted(v.begin(), v.end()));
#endif

		// do nothing if there is no data
		if (v.size() == 0) { return; }

		frequencies dummy{0,0,0};
		if (v.size() == 1) {
			m_root = [&]() {
				if constexpr (rvalue) {
					return insert(nullptr, m_root, std::forward<T>(v[0]), dummy);
				}
				else {
					return insert(nullptr, m_root, v[0], dummy);
				}
			}();
			return;
		}

		// Make a tree with the new info and then join the two trees.
		tree_node *n =
			_make_tree(std::forward<U>(v), 0, v.size() - 1, nullptr, '0');

		// if our root is empty then the new
		// node is the root of the new tree
		if (m_root == nullptr) {
			m_root = n;
			return;
		}

		// easy case, we only had one element in the tree
		if (m_root->tree_size == 1) {
			tree_node *r = insert(nullptr, n, std::move(m_root->key), dummy);
#if defined DEBUG
			assert(r != nullptr);
#endif
			{
			// update the counter of the leftmost node of
			// the tree rooted at 'r'.
			tree_node *lmost = r;
			while (lmost->left != nullptr) { lmost = lmost->left; }
			// obviously, copy the node_counter
			lmost->node_counter = m_root->node_counter;
			lmost->update_count();
			while (lmost->parent != nullptr) {
				lmost = lmost->parent;
				lmost->update_count();
			}
			}

			free_node(m_root);
			m_root = r;
			return;
		}

		// both 'root' and 'n' have size larger than 2
#if defined DEBUG
		assert(m_root->tree_size >= 2 and n->tree_size >= 2);
#endif
		m_root =
			(m_root->height >= n->height ?
				join_taller(m_root, n) :
				join_shorter(m_root, n)
			);
	}

	/**
	 * @brief Size of the tree.
	 * @returns The number of nodes in the tree.
	 */
	std::size_t num_nodes() const noexcept
	{ return (m_root == nullptr ? 0 : m_root->tree_size); }

	/**
	 * @brief Total number of elements inserted.
	 * @returns The total sum of occurrences of the elements in the tree.
	 */
	std::size_t total_elements() const noexcept
	{ return (m_root == nullptr ? 0 : m_root->tree_counter); }

#if defined __LAL_INSPECT
	/// Perform a series of correctness checks
	bool sanity_check() const noexcept {
		return sanity_check(m_root);
	}

	/// Print the tree and its contents to standard output and
	/// standard error output.
	void print_tree() const noexcept {
		print_tree(m_root, "");
	}
#endif

private:
	/// Node of the tree.
	struct tree_node {
		/// contents of the node
		T key;
		/// Amount of occurrences of @ref key.
		std::size_t node_counter = 0;

		/**
		 * @brief Amount of nodes in the subtree rooted at this node.
		 *
		 * Number of nodes in the left and right subtrees plus this node.
		 */
		std::size_t tree_size = 0;

		/**
		 * @brief Total number of occurrences in the nodes in the subtree rooted
		 * at this node.
		 *
		 * Sum of the occurrences at this node plus the sum of the occurrences
		 * across all nodes of the left subtree plus the sum of the occurrences
		 * across all nodes of the right subtree.
		 *
		 * This is not necessarily the same as @ref tree_size.
		 */
		std::size_t tree_counter = 0;

		/// Maximum height of the left and right subtrees' height
		std::size_t height = 0;
		/**
		 * @brief Balance factor of a node:
		 *
		 * Right subtree's height minus left subtree's height. This value is
		 * either -2, -1, 0, 1, 2.
		 */
		int64_t balance_factor = 0;

		/// Pointer to the parent of this node.
		tree_node *parent = nullptr;
		/// Pointer to the left subtree.
		tree_node *left = nullptr;
		/// Pointer to the right subtree.
		tree_node *right = nullptr;

		/**
		 * @brief Side of this node with respect to its parent.
		 *
		 * l: this node is a left subtree of its parent,
		 * r: this node is a right subtree of its parent,
		 * 0: this node is the root.
		 *		Eq. parent = nullptr
		 */
		char side = '0';

		/// Returns the size of the left subtree.
		[[nodiscard]] std::size_t left_size() const noexcept
		{ return (left == nullptr ? 0 : left->tree_size); }
		/// Returns the size of the right subtree.
		[[nodiscard]] std::size_t right_size() const noexcept
		{ return (right == nullptr ? 0 : right->tree_size); }

		/// Returns the total occurrences of the left subtree.
		[[nodiscard]] std::size_t left_counter() const noexcept
		{ return (left == nullptr ? 0 : left->tree_counter); }
		/// Returns the size of the right subtree.
		[[nodiscard]] std::size_t right_counter() const noexcept
		{ return (right == nullptr ? 0 : right->tree_counter); }

		/**
		 * @brief Calculate the height and balance factor of this node.
		 * @post Updates the @ref height and @ref balance_factor members.
		 */
		void update_height_and_balance_factor() noexcept {
			const int64_t lh = (left != nullptr ? to_int64(left->height) : -1);
			const int64_t rh = (right != nullptr ? to_int64(right->height) : -1);
			height = to_uint64(std::max(lh, rh) + 1);
			balance_factor = rh - lh;
		}

		/**
		 * @brief Computes the size of the subtree rooted at this node.
		 * @post Updates the @ref tree_size member.
		 */
		void update_size() noexcept {
			tree_size = 1 + left_size() + right_size();
		}

		/**
		 * @brief Computes the size of the subtree rooted at this node.
		 * @post Updates the @ref tree_counter member.
		 */
		void update_count() noexcept {
			tree_counter =
				node_counter +
				left_counter() +
				right_counter();
		}

		/**
		 * @brief Updates information of this node.
		 *
		 * - size (see @ref update_size),
		 * - count values (see @ref update_count),
		 * - the height (see @ref update_height_and_balance_factor).
		 */
		void update() noexcept {
			update_size();
			update_count();
			update_height_and_balance_factor();
		}

		/**
		 * @brief Replace this node with either its left or right child.
		 *
		 * Let p := this->parent be the parent of "this" node. That is, the
		 * grandparent of parameter @e n.
		 *
		 * If this node is a left child of 'p', make 'n' be the left child of
		 * 'p'. If it is a right child of 'p', make 'n' be the right child of
		 * 'p'.
		 * @param n Left or right child of this node.
		 * @post Reference @ref parent still points to the parent.
		 * @post If node 'n' is 'this->left', then reference @ref left still
		 * points to its left child.
		 * @post If node 'n' is 'this->right', then reference @ref right still
		 * points to its right child.
		 */
		void replace_with(tree_node *n) noexcept {
			if (parent != nullptr) {
				if (side == 'l') {
					parent->left = n;
				}
				else if (side == 'r') {
					parent->right = n;
				}
			}

			if (n == nullptr) { return; }

#if defined DEBUG
			assert(left == n or right == n);
#endif

			n->parent = parent;
			n->side = side;
		}
	};

private:
	/// Root of this AVL tree.
	tree_node *m_root = nullptr;

private:
	/// Deallocates the memory of node @e n.
	void free_node(tree_node *n) const noexcept {
		if (n == nullptr) { return; }
		free_node(n->left);
		free_node(n->right);
		delete n;
	}

	/* ------------------------------- */
	/* ROTATION AND BALANCING OF NODES */

	/**
	 * @brief Performs a right-rotation at node @e n.
	 * @param n Node.
	 * @returns The root of the new balanced tree after a right rotation.
	 * @pre Node @e n has a left subtree.
	 */
	[[nodiscard]] tree_node *right_rotation(tree_node *n) const noexcept {
#if defined DEBUG
		assert(n != nullptr);
#endif

		tree_node *P = n->parent;
		tree_node *L = n->left;

#if defined DEBUG
		assert(L != nullptr);
#endif

		// update n's parent
		//    (notice P might be null, however,
		//    it is null only when n->side != 'r' and 'l')
		if (n->side == 'r') { P->right = L; }
		else if (n->side == 'l') { P->left = L; }
		// parent of L is now parent of n
		L->parent = P;

		//     if n is the left child, then L is
		//     the left child, same for 'right'
		L->side = n->side;

		// update A's parent, and A's side
		n->parent = L;
		n->side = 'r';

		// update node E
		tree_node *E = L->right;
		n->left = E;
		if (E != nullptr) {
			E->side = 'l';
			E->parent = n;
		}
		L->right = n;

		// update nodes n ...
		n->update();
		// ... and L
		L->update();
		return L;
	}
	/**
	 * @brief Performs a left-rotation at node @e n.
	 * @param n Node.
	 * @returns The root of the new balanced tree after a left rotation.
	 * @pre Node @e n has a right subtree.
	 */
	[[nodiscard]] tree_node *left_rotation(tree_node *n) const noexcept {
#if defined DEBUG
		assert(n != nullptr);
#endif

		tree_node *R = n->right;

		// parent of n is now parent of R
		R->parent = n->parent;
		//    update R's parent
		if (n->side == 'r') { n->parent->right = R; }
		else if (n->side == 'l') { n->parent->left = R; }
		//     if R is the left child, then n is
		//     the left child, ...
		R->side = n->side;

		// update A's parent, and A's side
		n->parent = R;
		n->side = 'l';

		// update node E
		tree_node *E = R->left;
		n->right = E;
		if (E != nullptr) {
			E->side = 'r';
			E->parent = n;
		}
		R->left = n;

		// update nodes n ...
		n->update();
		// ... and R
		R->update();
		return R;
	}
	/**
	 * @brief Left-left imbalance case.
	 * @param n Node.
	 * @returns The root of the new balanced tree after a left-left rotation.
	 * @pre Node @e n has a left subtree.
	 */
	[[nodiscard]] tree_node *left_left_case(tree_node *n) const noexcept {
		return right_rotation(n);
	}
	/**
	 * @brief Left-right imbalance case.
	 * @param n Node.
	 * @returns The root of the new balanced tree after a left-right rotation.
	 * @pre Node @e n has a left subtree.
	 */
	[[nodiscard]] tree_node *left_right_case(tree_node *n) const noexcept {
		n->left = left_rotation(n->left);
		return right_rotation(n);
	}
	/**
	 * @brief Right-right imbalance case.
	 * @param n Node.
	 * @returns The root of the new balanced tree after a right-right rotation.
	 * @pre Node @e n has a right subtree.
	 */
	[[nodiscard]] tree_node *right_right_case(tree_node *n) const noexcept {
		return left_rotation(n);
	}
	/**
	 * @brief Right-left imbalance case.
	 * @param n Node.
	 * @returns The root of the new balanced tree after a right-left rotation.
	 * @pre Node @e n has a right subtree.
	 */
	[[nodiscard]] tree_node *right_left_case(tree_node *n) const noexcept {
		n->right = right_rotation(n->right);
		return left_rotation(n);
	}
	/**
	 * @brief Balance a node of the AVL tree.
	 *
	 * Uses the functions @ref left_left_case, @ref left_right_case,
	 * @ref right_right_case, @ref right_left_case.
	 * @param n Node to balance
	 * @returns The root of the new balanced tree.
	 */
	[[nodiscard]] tree_node *balance(tree_node *n) const noexcept {
		if (n == nullptr) { return nullptr; }
#if defined DEBUG
		assert(std::abs(n->balance_factor) <= 2);
#endif

		if (std::abs(n->balance_factor) <= 1) { return n; }
		return (
		n->balance_factor == -2 ?
			(n->left->balance_factor <= 0 ? left_left_case(n) : left_right_case(n)) :
			(n->right->balance_factor >= 0 ? right_right_case(n) : right_left_case(n))
		);
	}

	/* ----------------------------- */
	/* INSERTION OF A SINGLE ELEMENT */

	/**
	 * @brief Insert element 'x' to the tree.
	 * @param p Parent of node @e n
	 * @param n Node to which the element 'x' is to be assigned.
	 * @param x Element to be added.
	 * @param[out] freqs Frequencies associated to the value @e x.
	 * @returns The tree node containing as key the value @e x.
	 */
	template <typename U>
	[[nodiscard]]
	tree_node *insert(tree_node *p, tree_node *n, U&& x, frequencies& freqs)
	const noexcept
	{
		// Find where the node with key 'x' could be located in the tree.
		// If such vertex does not exist, then the result should be where
		// the new node should be located at.
		char side = '0';
		while (n != nullptr and (not (x == n->key))) {
			p = n;
			if (x < n->key) {
				// x < n->key
				freqs.counter_larger += n->node_counter + n->right_counter();
				freqs.num_nodes_larger += 1 + n->right_size();
				n = n->left;
				side = 'l';
			}
			else {
				// x > n->key
				n = n->right;
				side = 'r';
			}
		}

		const bool create_a_new_node = (n == nullptr);
		if (create_a_new_node) {
			// If the node does not exist, create a new one here
			n = new tree_node();
			n->key = std::forward<U>(x);
			n->left = n->right = nullptr;
			n->side = side;
			n->parent = p;
			if (side == 'l') { p->left = n; }
			else if (side == 'r') { p->right = n; }
			n->tree_size = n->node_counter = n->tree_counter = 1;
			n->height = 0;
			n->balance_factor = 0;
			freqs.counter_equal = 1;
		}
		else {
			// If the node exists, update frequencies.
#if defined DEBUG
			assert(n->key == x);
#endif
			n->node_counter += 1;
			n->tree_counter += 1;

			// accumulate frequencies
			freqs.counter_equal = n->node_counter;
			freqs.counter_larger += n->right_counter();
			freqs.num_nodes_larger += n->right_size();
		}

		// This is the case where the while loop did not execute
		// any iteration and 'n' was null since this function was
		// called. Simply return the newly-created node.
		if (side == '0') {
#if defined DEBUG
			assert(n != nullptr);
#endif
			return n;
		}

		if (create_a_new_node) {
			// Update node size, height, balance factor, ...
			// of all nodes from 'p' to the root all while
			// balancing the trees.
			while (p->parent != nullptr) {
				p->update();
				p = balance(p);
				p = p->parent;
			}
		}
		else {
			// No need to balance nodes in this case --
			// Traverse all the way up to the root
			while (p->parent != nullptr) {
				p->update();
				p = p->parent;
			}
		}

		p->update();
		return balance(p);
	}

	/* --------------------------- */
	/* REMOVAL OF A SINGLE ELEMENT */

	/**
	 * @brief Moves the contents of n to k, when appropriate
	 * @tparam use_counter_to_remove If true then this node is to be removed
	 * only if the counter has reached 0. If false, the node will be removed
	 * regardless of the value.
	 * @tparam move_in_leftmost Was this function called from @ref remove_leftmost?
	 * @param n Node of the tree.
	 * @param[out] k Copy of the leftmost node.
	 * @param[out] freqs The frequencies associated to the leftmost node.
	 * @returns True if the node is to be deleted; False if the node is not to
	 * be deleted.
	 */
	template <bool use_counter_to_remove, bool move_in_leftmost>
	bool delete_after_move(tree_node *n, tree_node *k, frequencies& freqs) const noexcept
	{
		freqs.counter_equal = n->node_counter;
		if constexpr (move_in_leftmost) {
			freqs.counter_larger += n->right_counter();
			freqs.num_nodes_larger += n->right_size();
		}

		bool delete_n = false;
		if constexpr (not use_counter_to_remove) {
			delete_n = true;
		}
		else {
			--n->node_counter;
			--n->tree_counter;
			delete_n = n->node_counter == 0;
		}

		if (k != nullptr) {
			k->node_counter = n->node_counter;
			if (delete_n) { k->key = std::move(n->key); }
			else { k->key = n->key; }
		}
		return delete_n;
	}

	/**
	 * @brief Remove the leftmost node in tree rooted at @e n.
	 * @tparam use_counter_to_remove If true then this node is to be removed
	 * only if the counter has reached 0. If false, the node will be removed
	 * regardless of the value.
	 * @param n Node of the tree.
	 * @param[out] k Copy of the leftmost node.
	 * @param[out] freqs The frequencies associated to the leftmost node.
	 * @returns The root of the tree resulting from the removal of the righmost
	 * node in tree @e n.
	 */
	template <bool use_counter_to_remove>
	[[nodiscard]]
	tree_node *remove_leftmost(tree_node *n, tree_node *k, frequencies& freqs)
	const noexcept
	{
		if (n == nullptr) { return nullptr; }
#if defined DEBUG
		assert(n != nullptr);
#endif

		const auto original = n;

		// special cases
		if (n->left == nullptr) {
			const auto delete_n =
				delete_after_move<use_counter_to_remove, true>(n, k, freqs);

			if (not delete_n) { return original; }

			if (n->parent == nullptr) {
				const auto l = n->right;
				delete n;
				return l;
			}

			auto p = n->parent;
			n->replace_with(n->right);
			delete n;
			return p->right;
		}

		// find the rightmost node
		while (n->left != nullptr) {
			freqs.counter_larger += n->node_counter + n->right_counter();
			freqs.num_nodes_larger += 1 + n->right_size();
			n = n->left;
		}
		// store the parent of n
		auto p = n->parent;

		// delete n, if appropriate
		const auto delete_n =
			delete_after_move<use_counter_to_remove, true>(n, k, freqs);

		if (delete_n) {
			n->replace_with(n->right);
			delete n;
		}

		// climb up the tree updating the nodes
		while (p != original) {
			p->update();
			p = balance(p);
			p = p->parent;
		}
		p->update();
		return balance(p);
	}
	/**
	 * @brief Remove the rightmost node in tree rooted at @e n.
	 * @tparam use_counter_to_remove If true then this node is to be removed
	 * only if the counter has reached 0. If false, the node will be removed
	 * regardless of the value.
	 * @param n Node of the tree.
	 * @param[out] k Copy of the rightmost node.
	 * @param[out] freqs The frequencies associated to the rightmost node.
	 * @returns The root of the tree resulting from the removal of the righmost
	 * node in tree @e n.
	 */
	template <bool use_counter_to_remove>
	[[nodiscard]]
	tree_node *remove_rightmost(tree_node *n, tree_node *k, frequencies& freqs)
	const noexcept
	{
		if (n == nullptr) { return nullptr; }

#if defined DEBUG
		assert(n != nullptr);
#endif

		const auto original = n;

		// special cases
		if (n->right == nullptr) {
			const auto delete_n =
				delete_after_move<use_counter_to_remove, false>(n, k, freqs);

			if (not delete_n) { return original; }

			if (n->parent == nullptr) {
				const auto l = n->left;
				delete n;
				return l;
			}

			auto p = n->parent;
			n->replace_with(n->left);
			delete n;
			return p->left;
		}

		// find the rightmost node
		while (n->right != nullptr) { n = n->right; }
		// store the parent of n
		auto p = n->parent;

		// delete n, if appropriate
		const auto delete_n =
			delete_after_move<use_counter_to_remove, false>(n, k, freqs);

		if (delete_n) {
			n->replace_with(n->left);
			delete n;
		}

		// climb up the tree updating the nodes
		while (p != original) {
			p->update();
			p = balance(p);
			p = p->parent;
		}
		p->update();
		return balance(p);
	}

	/**
	 * @brief Remove an element from the tree.
	 * @tparam use_counter_to_remove If true, the node is deleted only when
	 * the node counter reaches 0. If false, the node is always deleted.
	 * @param n Root node.
	 * @param x Element to be removed.
	 * @param[out] freqs Frequencies associated to the value @e x.
	 * @returns A tree node.
	 */
	template <bool use_counter_to_remove>
	[[nodiscard]]
	tree_node *remove(tree_node *n, const T& x, frequencies& freqs)
	const noexcept
	{
		if (n == nullptr) {
			// accumulate frequencies
			freqs.counter_equal = 0;
			return nullptr;
		}

		if (x < n->key) {
			// accumulate frequencies
			freqs.counter_larger += n->node_counter + n->right_counter();
			freqs.num_nodes_larger += 1 + n->right_size();

			// find the element in the left child.
			n->left = remove<use_counter_to_remove>(n->left, x, freqs);
			// update this node's size
			n->update();
			// balance 'n' to keep the AVL invariant
			return balance(n);
		}
		if (x > n->key) {
			// find the element in the right child.
			n->right = remove<use_counter_to_remove>(n->right, x, freqs);
			// update this node's size
			n->update();
			// balance 'n' to keep the AVL invariant
			return balance(n);
		}

		// found element at node 'n'
#if defined DEBUG
		assert(n->key == x);
#endif

		freqs.counter_equal = n->node_counter;
		freqs.counter_larger += n->right_counter();
		freqs.num_nodes_larger += n->right_size();

		bool completely_remove = false;
		if constexpr (not use_counter_to_remove) {
			// Remove the node with a complete disregard for occurrences.
			completely_remove = true;
		}
		else {
#if defined DEBUG
			assert(n->tree_counter > 0);
			assert(n->node_counter > 0);
#endif
			// Occurrences are important.
			n->tree_counter -= 1;
			n->node_counter -= 1;
			completely_remove = n->node_counter == 0;
		}

		if (completely_remove) {
			// update tree
			tree_node *L = n->left;
			tree_node *R = n->right;
			if (L == nullptr and R == nullptr) {
				// nothing else to do
				delete n;
				return nullptr;
			}
			if (L != nullptr and R == nullptr) {
				n->replace_with(L);
				delete n;
				// L is already balanced
				return L;
			}
			if (L == nullptr and R != nullptr) {
				n->replace_with(R);
				delete n;
				// R is already balanced
				return R;
			}

			// L != nullptr and R != nullptr

			// find the smallest value in the right subtree,
			// or the largest in the left subtree,
			// depending on the height
			frequencies dummy{0,0,0};
			if (L->height < R->height) {
				// the node must be moved, so we should not use the counter
				n->left = remove_rightmost<false>(L, n, dummy);
			}
			else {
				n->right = remove_leftmost<false>(R, n, dummy);
			}
		}

		n->update();
		return balance(n);
	}

	/* ----------------- */
	/* UNION OF TWO AVLS */

	/**
	 * @brief Join two AVL trees.
	 * @param T1 Taller tree.
	 * @param T2 Shorter tree.
	 * @returns A tree node.
	 * @pre The largest element in @e T1 is smaller than the smallest
	 * element in @e T2.
	 */
	[[nodiscard]]
	tree_node *join_taller(tree_node *T1, tree_node *T2) const noexcept {
#if defined DEBUG
		assert(T1 != nullptr);
		assert(T2 != nullptr);
		assert(T1->tree_size > 1 and T2->tree_size > 1);
#endif

		// we need a new node anyway
		tree_node *x = new tree_node();

		{
		// remove left-most element in T2
		frequencies dummy{0,0,0};
		T2 = remove_leftmost<false>(T2, x, dummy);
		}
		// find right-most node such that its height is either
		// (T2->height) or (T2->height + 1) in T1
		const std::size_t h = T2->height;
		tree_node *v = T1;
		std::size_t hp = v->height;
		while (hp > h + 1 and v != nullptr) {
			hp = (v->balance_factor == -1 ? hp - 2 : hp - 1);
			v = v->right;
		}

#if defined DEBUG
		assert(v != nullptr);
#endif

		// NOTE: 'u' is allowed to be nullptr!
		tree_node *u = v->parent;

		x->parent = u;
		x->left = v;
		x->right = T2;
		v->parent = x;
		v->side = 'l';
		T2->side = 'r';
		T2->parent = x;
		x->update();

		// this is why 'u' is allowed to be nullptr!
		if (u == nullptr) {
			x->side = '0';
			return balance(x);
		}

		u->right = x;
		x->side = 'r';

		// climb up the tree rebalancing
		x = balance(x);
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

	/**
	 * @brief Join two AVL trees.
	 * @param T1 Shorter tree.
	 * @param T2 Taller tree.
	 * @returns A tree node.
	 * @pre The largest element in @e T1 is smaller than the smallest
	 * element in @e T2.
	 */
	[[nodiscard]]
	tree_node *join_shorter(tree_node *T1, tree_node *T2) const noexcept {
#if defined DEBUG
		assert(T1 != nullptr);
		assert(T2 != nullptr);
		assert(T1->tree_size > 1 and T2->tree_size > 1);
#endif

		// we need a new node anyway
		tree_node *x = new tree_node();

		{
		// remove right-most element in T1
		frequencies dummy{0,0,0};
		T1 = remove_rightmost<false>(T1, x, dummy);
		}

		// find right-most node such that its height
		// is either (T1->height) or (T1->height + 1)
		// in T2
		const std::size_t h = T1->height;
		tree_node *v = T2;
		std::size_t hp = v->height;
		while (hp > h + 1 and v != nullptr) {
			hp = (v->balance_factor == -1 ? hp - 2 : hp - 1);
			v = v->left;
		}
#if defined DEBUG
		assert(v != nullptr);
#endif

		// NOTE: 'u' is allowed to be nullptr!
		tree_node *u = v->parent;

		x->parent = u;
		x->right = v;
		x->left = T1;
		v->parent = x;
		v->side = 'r';
		T1->side = 'l';
		T1->parent = x;
		x->update();

		// this is why 'u' is allowed to be nullptr!
		if (u == nullptr) {
			x->side = '0';
			return balance(x);
		}

		x->side = 'l';
		u->left = x;

		// climb up the tree rebalancing
		x = balance(x);
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

	/**
	 * @brief Make an AVL tree from the elements in @e v
	 *
	 * This function performs a recursive binary search over @e v. The limits
	 * of this search are @e l and @e r. At each call, this function creates a
	 * @ref tree_node with the value in the midpoint between @e l and @e r.
	 * @param v Vector sorted non-decreasingly.
	 * @param l Left-limit of the binary search-
	 * @param r Right-limit of the binary search.
	 * @param p Parent of the new node to be created.
	 * @param s Side of the node to create with respect to the parent.
	 */
	template <typename U>
	[[nodiscard]] tree_node *_make_tree
	(U&& v, std::size_t l, std::size_t r, tree_node *p, char s)
	const noexcept
	{
		using elem_t = typename std::remove_reference_t<U>::value_type;
		static constexpr bool rvalue = std::is_same_v<std::vector<elem_t>, U>;

		// middle point
		const std::size_t m = (l + r)/2;
		// make a node with the element in the middle
		tree_node *n = new tree_node();
		if constexpr (rvalue) {
			n->key = std::forward<T>(v[m]);
		}
		else {
			n->key = v[m];
		}
		// make sure pointers are correct
		n->parent = p;
		n->side = s;
		// construct the subtrees
		n->left = (l + 1 > m ?
				   nullptr : _make_tree(std::forward<U>(v), l, m - 1, n, 'l'));
		n->right = (m + 1 > r ?
					nullptr : _make_tree(std::forward<U>(v), m + 1, r, n, 'r'));
		// update the number of occurrences
		n->node_counter = 1;
		n->update();
		// by construction, there is no need to balance the node 'n'
		return n;
	}

private:

#if defined __LAL_INSPECT
	std::size_t exhaustive_size(tree_node *n) const noexcept {
		if (n == nullptr) { return 0; }
		return 1 + exhaustive_size(n->right) + exhaustive_size(n->left);
	}
	std::size_t exhaustive_occurrences(tree_node *n) const noexcept {
		if (n == nullptr) { return 0; }
		return n->node_counter +
				exhaustive_occurrences(n->right) +
				exhaustive_occurrences(n->left);
	}
	std::size_t exhaustive_height(tree_node *n) const noexcept {
		if (n == nullptr) { return 0; }
		if (n->left == nullptr and n->right == nullptr) { return 0; }
		const auto height_left = exhaustive_height(n->left);
		const auto height_right = exhaustive_height(n->right);
		return 1 + std::max(height_left, height_right);
	}

	bool all_smaller_than(tree_node *n, const T& x) const noexcept {
		if (n == nullptr) { return true; }
		if (n->key > x) { return false; }
		return all_smaller_than(n->left, x) and all_smaller_than(n->right, x);
	}
	bool all_greater_than(tree_node *n, const T& x) const noexcept {
		if (n == nullptr) { return true; }
		if (n->key < x) { return false; }
		return all_greater_than(n->left, x) and all_greater_than(n->right, x);
	}
	bool check_relations(tree_node *n) const noexcept
	{
		if (n == nullptr) { return true; }
		const bool smaller_left = all_smaller_than(n->left, n->key);
		const bool greater_right = all_greater_than(n->right, n->key);
		if (not smaller_left or not greater_right) { return false; }
		return check_relations(n->left) and check_relations(n->right);
	}
	bool sanity_check(tree_node *n) const noexcept {
		if (n == nullptr) { return true; }

		if (not check_relations(n)) {
			std::cerr
				<< "Elements incorrectly placed in the tree.\n"
				<< "    n->key= " << n->key << '\n';
			return false;
		}

		if (std::abs(n->balance_factor) >= 2) {
			std::cerr
				<< "Incorrect balance factor.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->balance_factor= " << n->balance_factor << '\n';
			return false;
		}

		// HEIGHT
		{
		const auto my_height = exhaustive_height(n);
		if (my_height != n->height) {
			std::cerr
				<< "Incorrect height.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->height= " << n->height << '\n'
				<< "    my_height= " << my_height << '\n';
			return false;
		}
		}

		// SIZES
		{
		const auto my_size = exhaustive_size(n);
		if (my_size != n->tree_size) {
			std::cerr
				<< "Incorrect sizes.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->tree_size= " << n->tree_size << '\n'
				<< "    my_size=      " << my_size << '\n';
			return false;
		}
		}

		{
		const auto my_size = exhaustive_size(n->left);
		if (my_size != n->left_size()) {
			std::cerr
				<< "Incorrect sizes.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->left_size()= " << n->left_size() << '\n'
				<< "    my_size=        " << my_size << '\n';
			return false;
		}
		}

		{
		const auto my_size = exhaustive_size(n->right);
		if (my_size != n->right_size()) {
			std::cerr
				<< "Incorrect sizes.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->right_size()= " << n->right_size() << '\n'
				<< "    my_size=         " << my_size << '\n';
			return false;
		}
		}

		// OCCURRENCES
		{
		const auto my_occurrences = exhaustive_occurrences(n);
		if (my_occurrences != n->tree_counter) {
			std::cerr
				<< "Incorrect occurrencess.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->tree_occurrences= " << n->tree_counter << '\n'
				<< "    my_occurrences=      " << my_occurrences << '\n';
			return false;
		}
		}

		{
		const auto my_occurrences = exhaustive_occurrences(n->left);
		if (my_occurrences != n->left_counter()) {
			std::cerr
				<< "Incorrect occurrencess.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->left_occurrences()= " << n->left_counter() << '\n'
				<< "    my_occurrences=        " << my_occurrences << '\n';
			return false;
		}
		}

		{
		const auto my_occurrences = exhaustive_occurrences(n->right);
		if (my_occurrences != n->right_counter()) {
			std::cerr
				<< "Incorrect occurrencess.\n"
				<< "    n->key= " << n->key << '\n'
				<< "    n->right_occurrences()= " << n->right_counter() << '\n'
				<< "    my_occurrences=         " << my_occurrences << '\n';
			return false;
		}
		}

		if (n->left != nullptr) {
			if (n->left->key > n->key) {
				std::cerr
					<< "Keys do not satisfy the order requirement.\n"
					<< "    n->key= " << n->key << '\n'
					<< "    n->left->key= " << n->left->key << '\n'
					<< "    n->key=       " << n->key << '\n';
				return false;
			}

			if (n->left->side != 'l') {
				std::cerr << "Wrong side for left child: '" << n->left->side << ".\n";
				return false;
			}
		}

		if (n->right != nullptr) {
			if (n->right->key < n->key) {
				std::cerr
					<< "Keys do not satisfy the order requirement.\n"
					<< "    n->key= " << n->key << '\n'
					<< "    n->right->key= " << n->right->key
					<< "    n->key=        " << n->key << '\n';
				return false;
			}

			if (n->right->side != 'r') {
				std::cerr << "Wrong side for right child: '" << n->right->side << ".\n";
				return false;
			}
		}

		return sanity_check(n->left) and sanity_check(n->right);
	}

	void print_tree(tree_node *n, const std::string& tab) const noexcept {
		std::cout << tab;

		if (n == nullptr) {
			std::cout << "∅\n";
			return;
		}

		std::cout
			<< n->key
			<< ", nc= " << n->node_counter
			<< ", s= " << n->side
			<< ", ls= " << n->left_size()
			<< ", rs= " << n->right_size()
			<< ", h= " << n->height
			<< ", ts= " << n->tree_size
			<< ", tc= " << n->tree_counter
			<< ", bf= " << n->balance_factor
			<< '\n';
		print_tree(n->left,  tab + "| -l-: ");
		print_tree(n->right, tab + "| +r+: ");
	}
#endif
};

template <typename T>
using AVL_frequencies = typename AVL<T>::frequencies;

} // -- namespace detail
} // -- namespace lal
