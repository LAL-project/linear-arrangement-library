/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <optional>
#include <vector>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Rooted tree graph class.
 *
 * This class provides its users with an abstraction of rooted trees. Rooted
 * trees are free trees in which one vertex has been designated as the root.
 * Furthermore, in the context of this library, these trees' edges are always
 * oriented towards the leaves (away from the root); this is known as an
 * arborescence. Many methods require objects of this class to be valid rooted
 * trees: the object must be a tree (see @ref is_tree), must have a root (see
 * @ref has_root), and be a valid rooted tree (see @ref is_rooted_tree).
 *
 * Rooted trees can be constructed in two different ways:
 * - Using an already-constructed free tree via a class constructor, where
 * users have to indicate the root (either
 * @ref rooted_tree(const lal::graphs::free_tree&, node, bool, bool)
 * or @ref rooted_tree(lal::graphs::free_tree&&, node, bool, bool)).
 * Alternatively, one can use the method @ref init_rooted, which has the same set
 * of parameters.
 * - Adding edge after edge. In this class, as in @ref lal::graphs::free_tree,
 * this addition is constrained so that the underlying undirected graph does not
 * contain cycles. Before (or after) the addition of all the edges, it is
 * recommended the root be set using @ref set_root.
 *
 * Adding edges one by one has a serious drawback. In case the edges do not
 * have a consistent orientation (either always pointing away from the root
 * or always pointing towards it), the resulting graph is not considered to be
 * a valid rooted tree (see @ref is_rooted_tree). Therefore, consider the use
 * of methods @ref can_add_edge or @ref can_add_edges. Recall that removal of
 * edges is allowed at every moment.
 *
 * The root allows defining further properties on these graphs. For example,
 * the user can query information regarding subtrees of a particular rooted tree
 * (see methods @ref get_num_nodes_subtree, @ref calculate_size_subtrees,
 * @ref get_edges_subtree, and @ref get_subtree). Not every vertex can be a root
 * of the tree: in general, only those vertices with in-degree 0 can
 * (see @ref is_root_valid).
 *
 * This class allows flexibility of use of rooted trees regarding the root's
 * choice (within the valid possibilities). Method @ref set_root allows changing
 * the root of rooted trees multiple times and at any time. However, when the
 * tree has all of its edges then only one vertex can be the root (that with
 * in-degree 0). For this reason, in case a user wants to build "different rooted
 * trees on different roots", it is strongly recommended that first a
 * @ref lal::graphs::free_tree is built, and then create a rooted tree using one
 * of the two constructors with a free tree (either
 * @ref rooted_tree(const lal::graphs::free_tree&, node, bool, bool), or
 * @ref rooted_tree(lal::graphs::free_tree&&, node, bool, bool)), or the method
 * @ref init_rooted.
 */
class rooted_tree : public directed_graph, virtual public tree {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	rooted_tree() noexcept : tree(), directed_graph() { }
	/**
	 * @brief Constructor with number of nodes and root node.
	 * @param n Number of vertices.
	 */
	rooted_tree(const uint64_t n) noexcept {
		rooted_tree::_init(n);
	}

	/**
	 * @brief Copy constructor with directed graph.
	 * @param t A directed graph.
	 * @pre Graph @e t is a tree.
	 */
	rooted_tree(const directed_graph& t) noexcept;

	/**
	 * @brief Move constructor with directed graph.
	 * @param t A directed graph.
	 * @pre Graph @e t is a tree.
	 */
	rooted_tree(directed_graph&& t) noexcept;

	/**
	 * @brief Copy constructor.
	 * @param r Rooted tree.
	 */
	rooted_tree(const rooted_tree& r) noexcept : graph(), tree(), directed_graph() {
		copy_full_rooted_tree(r);
	}

	/**
	 * @brief Move constructor.
	 * @param r Rooted tree.
	 */
	rooted_tree(rooted_tree&& r) noexcept {
		move_full_rooted_tree(std::forward<rooted_tree>(r));
	}

	/**
	 * @brief Constructor with free tree and root node.
	 * @param t Free tree.
	 * @param r Root node.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre Tree @e t is a valid free tree.
	 */
	rooted_tree
	(
		const free_tree& t,
		const node r,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept
	{
		init_rooted(t, r, norm, check_norm);
	}

	/**
	 * @brief Constructor with tree and root node.
	 * @param t Free tree.
	 * @param r Root node.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre Tree @e t is a valid free tree.
	 * @post Tree @e t is moved and should not be used.
	 */
	rooted_tree
	(
		free_tree&& t,
		const node r,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept
	{
		init_rooted(std::forward<free_tree>(t), r, norm, check_norm);
	}

	/// Destructor
	~rooted_tree() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Copy assignment operator.
	 * @param r Rooted tree.
	 */
	rooted_tree& operator= (const rooted_tree& r) noexcept {
		copy_full_rooted_tree(r);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param r Rooted tree.
	 */
	rooted_tree& operator= (rooted_tree&& r) noexcept {
		move_full_rooted_tree(std::forward<rooted_tree>(r));
		return *this;
	}

	/* MODIFIERS */

	/**
	 * @brief Initializer with tree and root node.
	 *
	 * Constructs a rooted tree from a free tree and one of its nodes as
	 * the root of the rooted tree.
	 *
	 * Since the edges are oriented, method @ref is_tree must be true on
	 * parameter @e t (otherwise, some edges might not be reachable from
	 * the root and hence completely undirectable).
	 * @param t Undirected tree.
	 * @param r Root of the rooted tree. A node of @e g.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre Parameter @e t must be a tree (see @ref is_tree).
	 * @post Method @ref is_rooted_tree returns true.
	 */
	void init_rooted
	(
		const free_tree& t,
		const node r,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Initializer with tree and root node.
	 *
	 * Constructs a rooted tree from a free tree and one of its nodes as
	 * the root of the rooted tree.
	 *
	 * Since the edges are oriented, method @ref is_tree must be true on
	 * parameter @e t (otherwise, some edges might not be reachable from
	 * the root and hence completely undirectable).
	 * @param t Undirected tree.
	 * @param r Root of the rooted tree. A node of @e g.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre Parameter @e t must be a tree (see @ref is_tree).
	 * @post Method @ref is_rooted_tree returns true.
	 */
	void init_rooted
	(
		free_tree&& t,
		const node r,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	rooted_tree& add_node() noexcept {
		directed_graph::add_node();
		tree_only_add_node();
		m_size_subtrees.push_back(1);
		m_are_size_subtrees_valid = false;
		return *this;
	}

	/**
	 * @brief Remove a node from this tree.
	 *
	 * @param u Valid node index: \f$0 \le u < n\f$.
	 * @param connect If connect is true then the parent of @e u is connected
	 * to the children of @e u, if both parent and children exist.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The node must exist.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the removal of the node.
	 * @post If @e u is the root of this tree, then this tree no longer has a root.
	 */
	rooted_tree& remove_node
	(
		const node u,
		const bool connect = false,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Adds an edge to the tree.
	 *
	 * This operation checks that the edge added does not produce cycles
	 * only in a @e debug compilation of the library. For a more controlled
	 * addition of the edges, see @ref can_add_edge.
	 *
	 * <b>For developers:</b> method @ref lal::graphs::graph::actions_after_add_edge is
	 * called after the edge has been added.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Should the graph be normalized?
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre \f$s \neq t\f$
	 * @pre Edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	rooted_tree& add_edge
	(
		const node s,
		const node t,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Adds an edge to the graph.
	 *
	 * This method only adds an edge, and does no other work: normalisation
	 * is not checked, and no extra work per edge is done.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @pre \f$u \neq v\f$.
	 * @pre The edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	rooted_tree& add_edge_bulk(const node s, const node t) noexcept;

	/**
	 * @brief Finishes adding edges in bulk.
	 *
	 * This method updates the Union-Find data structure and all the necessary
	 * members after several calls to @ref add_edge_bulk.
	 * @param norm Normalize the tree.
	 * @param check Check whether the tree is normalized or not.
	 */
	void finish_bulk_add(const bool norm = true, const bool check = true) noexcept;

	void finish_bulk_add_complete(const bool norm = true, const bool check = true) noexcept;

	/**
	 * @brief Adds a list of edges to the graph.
	 *
	 * This function checks that edges will not produce cycles only in a
	 * @e debug compilation of the library. Moreover, this operation is
	 * faster than calling @ref add_edge since the edges are added in bulk.
	 * For a more controlled addition of the edges, see @ref can_add_edges.
	 * @param edges The edges to be added.
	 * @param norm Normalize the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge.
	 * @pre None of the subsets of the list of edges can produce cycles
	 * when added.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edges.
	 */
	rooted_tree& add_edges
	(
		const std::vector<edge>& edges,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Sets the edges to the graph.
	 *
	 * Sets the edges of this graph assuming that the nodes indexed in the
	 * list are, at most, the number of nodes of this graph.
	 *
	 * This list of edges is assumed to be all the edges that are going
	 * to be added to this graph. This means that the internal data structures
	 * are constructed more efficiently than when adding edges one by one
	 * (see @ref add_edge) or in several chunks (see @ref add_edges).
	 * For a more controlled addition of the edges, see @ref can_add_edges.
	 *
	 * Moreover, the current structure of the graph is cleared before setting
	 * the new edges.
	 * @param edges The edges to be added.
	 * @param norm Normalize the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The graph has been initialized with as many nodes as vertices in the
	 * list of edges.
	 * @pre There are no repeated edges in the list.
	 * @pre The list of edges must form a valid rooted tree, i.e., there must
	 * be a unique vertex with no in-going edges, there must be no cycles, and
	 * every vertex has in-degree at most 1.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 * @post The tree has a valid root which is, potentially, different from
	 * the previous root it had. Therefore, method @ref has_root returns true.
	 * @post Method @ref is_rooted_tree returns true.
	 */
	rooted_tree& set_edges
	(
		const std::vector<edge>& edges,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * <b>For developers:</b> method @ref lal::graphs::graph::actions_after_remove_edge
	 * is called after the edge has been removed.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The edge must exist.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	rooted_tree& remove_edge
	(
		const node s,
		const node t,
		const bool norm = false,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Removes an edge from the tree.
	 *
	 * This method only removes an edge, and does no other work: normalisation
	 * is not checked, and no extra work per edge is done.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @pre \f$u \neq v\f$.
	 * @pre The edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the removal of the edge.
	 */
	rooted_tree& remove_edge_bulk(const node s, const node t) noexcept;

	/**
	 * @brief Finishes removing edges in bulk.
	 *
	 * This method updates the Union-Find data structure and all the necessary
	 * members after several calls to @ref remove_edge_bulk.
	 * @param norm Normalize the tree.
	 * @param check Check whether the tree is normalized or not.
	 */
	void finish_bulk_remove(const bool norm = true, const bool check = true) noexcept;

	void finish_bulk_remove_complete(const bool norm = true, const bool check = true) noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed
	 * in bulk.
	 * @param edges The edges to be deleted.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge(node,node,bool,bool).
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	rooted_tree& remove_edges
	(
		const std::vector<edge>& edges,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Remove all edges incident to a given vertex.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed
	 * in bulk.
	 * @param u The node whose incident vertices are to be removed.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	rooted_tree& remove_edges_incident_to
	(
		const node u,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Disjoint union of trees.
	 *
	 * Append a rooted tree to this tree. All the nodes in @e t are relabelled
	 * starting at @e n, the number of nodes of the current tree. If the
	 * current graph has no vertices, then the whole input tree (and its state)
	 * is simply copied into this tree.
	 *
	 * @param t Input tree.
	 * @param connect_roots The root of the current tree and the root of
	 * @e t are joined by an edge.
	 * @pre If @e connect_roots is true then both trees need to have a root
	 * (see method @ref has_root).
	 * @post The root (if set) of the current tree is kept.
	 * @post The size of the subtrees might need recalculating:
	 * - If method @ref are_size_subtrees_valid() returns true for both trees then
	 * the subtree sizes are updated and do not need to be recalculated and
	 * method @ref are_size_subtrees_valid() still returns true.
	 * - If for one of the two graphs the method @ref are_size_subtrees_valid()
	 * returns false then it will still return false after this operation.
	 * @post The graph resulting from the union is normalized only if the
	 * two graphs were normalized prior to the union.
	 * @post The type of tree is invalidated.
	 */
	rooted_tree& disjoint_union(const rooted_tree& t, const bool connect_roots = true)
	noexcept;

	/**
	 * @brief Calculates the number of nodes at every rooted subtree.
	 * @pre The object must be a tree (see @ref is_tree).
	 * @pre The tree must have a root (see @ref has_root).
	 * @post Method @ref are_size_subtrees_valid returns true.
	 */
	void calculate_size_subtrees() noexcept;

	void calculate_tree_type() noexcept;

	/* SETTERS */

	/**
	 * @brief Set the root of this tree.
	 *
	 * Changing the root of a rooted tree invalidates information dependant
	 * on the tree. See the postconditions for details.
	 * @param r Vertex that represents the root.
	 * @pre The adjacency list must have been initialized.
	 * @post Method @ref has_root returns true.
	 * @post The type of rooted tree and the size of the subtrees are
	 * invalidated.
	 */
	void set_root(const node r) noexcept {
		// if the tree is empty simply consider it has a root...
		// although it really doesn't

		if (get_num_nodes() > 0) {
#if defined DEBUG
			assert(has_node(r));
			assert(is_root_valid(r));
#endif
			m_root = r;
		}
		m_are_size_subtrees_valid = false;
		tree_only_invalidate();
	}

	/* GETTERS */

	/**
	 * @brief Is the root valid?
	 *
	 * A root is valid if it has in-degree 0. This is calculated as a function
	 * of the current state of the tree.
	 * @param r Given node.
	 * @returns Whether or not the node passed as parameter is a valid root.
	 */
	[[nodiscard]] bool is_root_valid(const node r) const noexcept {
#if defined DEBUG
		assert(has_node(r));
#endif
		return get_in_degree(r) == 0;
	}

	[[nodiscard]] bool can_add_edge(const node s, const node t) const noexcept;

	[[nodiscard]] bool can_add_edges(const std::vector<edge>& edges) const noexcept;

	[[nodiscard]] bool is_rooted() const noexcept { return true; }

	/**
	 * @brief Is this tree a valid rooted tree?
	 *
	 * A tree is a valid rooted tree when:
	 * - the underlying undirected graph is connected and does not contain
	 * cycles (see @ref is_tree),
	 * - the tree has a root (see @ref has_root, @ref set_root, @ref get_root).
	 *
	 * @returns Whether this tree is a valid rooted tree or not.
	 */
	[[nodiscard]] bool is_rooted_tree() const noexcept { return is_tree() and has_root(); }

	/// Return the root of this tree.
	[[nodiscard]] node get_root() const noexcept {
#if defined DEBUG
		assert(has_root());
#endif
		return *m_root;
	}
	/// Returns whether this rooted tree's root has been set or not
	/// (see @ref set_root).
	[[nodiscard]] bool has_root() const noexcept { return m_root.has_value(); }

	/**
	 * @brief Get the size of a subtree rooted at a given node.
	 * @param u Vertex of the tree.
	 * @returns The number of nodes of the subtree rooted at @e u.
	 * @pre Method @ref are_size_subtrees_valid returns true.
	 */
	[[nodiscard]] uint64_t get_num_nodes_subtree(const node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
		assert(are_size_subtrees_valid());
#endif
		return m_size_subtrees[u];
	}

	/**
	 * @brief Is a recalculation of the subtree's sizes needed?
	 *
	 * If the method returns @e false then the user should call
	 * @ref calculate_size_subtrees so that the size of every rooted subtree
	 * is recalculated. This information must be calculated prior to calling
	 * many functions of this library.
	 * @returns Whether @ref m_size_subtrees should be recalculated
	 * or not.
	 */
	[[nodiscard]] bool are_size_subtrees_valid() const noexcept
	{ return m_are_size_subtrees_valid; }

	/**
	 * @brief Retrieve the edges of the subtree rooted at @e u.
	 *
	 * The list of edges returned contains labels that depend on the parameter
	 * @e relab. If @e relab is true then the nodes are relabelled to
	 * numbers in \f$[0, n_u)\f$, where \f$n_u\f$ is the number of nodes
	 * of the subtree rooted at @e u, rather than keeping the original
	 * labelling of numbers in \f$[0,n)\f$, where @e n is the number of
	 * nodes of the tree.
	 *
	 * For example, consider the following complete binary tree of 7 nodes,
	 * whose edges are
	 * <pre>
	 * 0 -> 1 -> 3
	 *        -> 4
	 *   -> 2 -> 5
	 *        -> 6
	 * </pre>
	 * The edges of the subtree rooted at 1 are "1 -> 3" and "1 -> 4", or, for
	 * the mathematically inclined \f$(1,3), (1,4)\f$.
	 *
	 * This method can be seen as a way of relabelling nodes when @e u is
	 * the root of the tree and @e relab is true.
	 * @param u Root node of the subtree.
	 * @param relab Should the nodes be relabelled?
	 * @returns A list of edges.
	 * @pre The object must be a valid rooted tree (see @ref is_rooted_tree).
	 * @post Whenever @e relab is true, the label of the first node of
	 * the first edge is guaranteed to be node '0'.
	 */
	[[nodiscard]] std::vector<edge> get_edges_subtree(const node u, const bool relab = false)
	const noexcept;

	/**
	 * @brief Retrieve the subtree rooted at node @e u.
	 * @param u Root of the subtree.
	 * @returns A tree containing the nodes of the subtree
	 * rooted at node @e u.
	 * @pre The object must be a valid rooted tree (see @ref is_rooted_tree).
	 */
	[[nodiscard]] rooted_tree get_subtree(const node u) const noexcept;

	/**
	 * @brief Converts this rooted tree into a free tree (see @ref free_tree).
	 * @param norm Normalize the tree.
	 * @param check Chech whether the resulting graph is normalized or not.
	 */
	[[nodiscard]] free_tree to_free_tree
	(const bool norm = true, const bool check = true)
	const noexcept;

	/**
	 * @brief Converts a rooted tree into a head vector.
	 *
	 * See @ref LAL_concepts__head_vector page for a definition of 'head vector'.
	 *
	 * @param arr Linear arrangement of the tree.
	 * @returns The head vector representation of this tree.
	 * @pre This tree is a valid rooted tree (see @ref is_rooted_tree).
	 */
	[[nodiscard]] head_vector get_head_vector(const linear_arrangement& arr = {})
	const noexcept;

	/**
	 * @brief Does the subtree rooted at @e r contain node @e u?
	 * @param r The root of the subtree. Notice that if @e r is the actual root
	 * of the subtree, the tree certainly contains @e u.
	 * @param u Node to query within the subtree.
	 * @returns Whether or not the subtree rooted at @e r contains node łe u.
	 * @pre Node @e r belongs to the tree (see @ref has_node).
	 * @pre Node @e u belongs to the tree (see @ref has_node).
	 * @pre This tree is a valid rooted tree (see @ref is_rooted_tree).
	 */
	[[nodiscard]] bool subtree_contains_node(const node r, const node u) const noexcept;

	/**
	 * @brief Are two nodes siblings?
	 *
	 * Do to two nodes share the same parent?
	 * @param u A node.
	 * @param v Another node.
	 * @returns True if the nodes are siblings. False, if they are not.
	 */
	[[nodiscard]] bool are_nodes_siblings(const node u, const node v) const noexcept {
		// if one of the in-degrees is zero, then 'u' and 'v' cannot be siblings
		if (get_in_degree(u) == 0 or get_in_degree(v) == 0) {
			return false;
		}
		// two nodes are siblings when their respective parent vertices
		// are the same
		return get_in_neighbors(u)[0] == get_in_neighbors(v)[0];
	}

	/**
	 * @brief Does a node have a parent vertex?
	 * @param u Input node.
	 * @returns If the node has a parent vertex.
	 */
	[[nodiscard]] bool node_has_parent(const node u) const noexcept {
		return get_in_degree(u) > 0;
	}

	/**
	 * @brief Parent vertex of a node.
	 * @param u Input node.
	 * @returns The parent vertex of a node.
	 * @pre Method @ref node_has_parent must return true.
	 */
	[[nodiscard]] node get_parent_node(const node u) const noexcept {
#if defined DEBUG
		assert(node_has_parent(u));
#endif
		return get_in_neighbors(u)[0];
	}

protected:
	/// Root of the tree.
	std::optional<node> m_root;

	/**
	 * @brief Number of nodes of the subtrees rooted at a certain node.
	 *
	 * Given a node @e u, @ref m_size_subtrees[u] gives the number
	 * of nodes of the subtree rooted at @e u.
	 */
	std::vector<uint64_t> m_size_subtrees;
	/// Are the contents of @ref m_size_subtrees valid?
	bool m_are_size_subtrees_valid = false;

protected:
	/**
	 * @brief Initializes the memory in the graph hierarchy.
	 *
	 * Initializes memory of @ref lal::graphs::free_tree,
	 * @ref lal::graphs::undirected_graph and @ref lal::graphs::graph classes.
	 * @param n Number of nodes.
	 * @pre The graph is cleared.
	 */
	void _init(const uint64_t n) noexcept {
		tree::tree_only_init(n);
		directed_graph::_init(n);
		m_size_subtrees.resize(n);
		m_are_size_subtrees_valid = false;
		if (n <= 1) {
			set_root(0);
		}
	}
	/**
	 * @brief Clears the memory in the graph hierarchy.
	 *
	 * Clears the memory of @ref lal::graphs::free_tree,
	 * @ref lal::graphs::undirected_graph and @ref lal::graphs::graph classes.
	 */
	void _clear() noexcept {
		tree::tree_only_clear();
		directed_graph::_clear();
		m_size_subtrees.clear();
		m_are_size_subtrees_valid = false;
	}

	void actions_after_add_edge(const node u, const node v) noexcept;

	void actions_after_add_edges(const edge_list& e) noexcept;

	void actions_after_add_edges_bulk() noexcept;

	void actions_after_remove_edge(const node u, const node v) noexcept;

	void actions_after_remove_edges(const edge_list& e) noexcept;

	void actions_after_remove_edges_bulk() noexcept;

	void actions_before_remove_edges_incident_to(const node u) noexcept;

	void actions_after_remove_node(const node u) noexcept;

	void update_union_find_after_add_edge
	(
		const node u,
		const node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;

	void update_union_find_after_add_edges
	(
		const edge_list& edges,
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;

	void update_union_find_after_add_edges_bulk
	(
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;

	void update_union_find_after_remove_edge
	(
		const node u,
		const node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;
	
	void update_union_find_after_remove_edges
	(
		const edge_list& edges,
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;

	void update_union_find_after_remove_edges_bulk
	(
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;

	void update_union_find_before_remove_incident_edges_to(
		const node u,
		uint64_t * const root_of,
		uint64_t * const root_size
	)
	const noexcept;

	/// Copies all members of this class and the parent classes.
	void copy_full_rooted_tree(const rooted_tree& r) noexcept {
		// copy directed_graph class
		copy_full_directed_graph(r);

		// copy only tree's members
		tree_only_copy(r);

		// copy this class' members
		m_root = r.m_root;
		m_size_subtrees = r.m_size_subtrees;
		m_are_size_subtrees_valid = r.m_are_size_subtrees_valid;
	}
	/// Moves all members of this class and the parent classes.
	void move_full_rooted_tree(rooted_tree&& r) noexcept {
		// move-assign directed_graph class
		move_full_directed_graph(std::forward<rooted_tree>(r));

		// move-assign only tree's members
		tree_only_move(std::forward<tree>(r));

		// move this class' members
		m_root.swap(r.m_root);
		m_size_subtrees = std::move(r.m_size_subtrees);
		m_are_size_subtrees_valid = r.m_are_size_subtrees_valid;

		r.m_root.reset(); // old tree cannot have a root anymore.. it's been moved!
		r.m_are_size_subtrees_valid = false;
	}

private:
	using directed_graph::disjoint_union;
	using directed_graph::remove_node;
};

} // -- namespace graphs
} // -- namespace lal
