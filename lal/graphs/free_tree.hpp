/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Free tree graph class.
 *
 * This class constrains the addition of edges so that the resulting graphs
 * does not contain cycles. Furthermore, the edges added are undirected.
 *
 * For another type of tree-like graphs, see @ref rooted_tree.
 */
class free_tree : public undirected_graph, virtual public tree {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	free_tree() noexcept : tree(), undirected_graph() { }
	/**
	 * @brief Constructor with number of vertices.
	 * @param n Number of vertices
	 */
	free_tree(uint64_t n) noexcept : undirected_graph(n) {
		free_tree::tree_only_init(n);
	}
	/**
	 * @brief Copy constructor.
	 * @param t Free tree.
	 */
	free_tree(const free_tree& t) noexcept : graph(), tree(), undirected_graph() {
		copy_full_free_tree(t);
	}

	/**
	 * @brief Move constructor.
	 * @param t Free tree.
	 */
	free_tree(free_tree&& t) noexcept {
		move_full_free_tree(std::forward<free_tree>(t));
	}

	/**
	 * @brief Copy constructor with undirected graph.
	 * @param t An undirected graph.
	 * @pre Graph @e t is a tree.
	 */
	free_tree(const undirected_graph& t) noexcept;

	/**
	 * @brief Move constructor with undirected graph.
	 * @param t An undirected graph.
	 * @pre Graph @e t is a tree.
	 */
	free_tree(undirected_graph&& t) noexcept;

	/// Destructor.
	virtual ~free_tree() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Copy assignment operator.
	 * @param f A lal::graphs::free_tree.
	 */
	free_tree& operator= (const free_tree& f) noexcept {
		copy_full_free_tree(f);
		return *this;
	}
	/**
	 * @brief Copy assignment operator.
	 * @param f A lal::graphs::free_tree.
	 */
	free_tree& operator= (free_tree&& f) noexcept {
		move_full_free_tree(std::forward<free_tree>(f));
		return *this;
	}

	/* MODIFIERS */

	virtual free_tree& remove_node
	(node u, bool norm = true, bool check_norm = true) noexcept;

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
	 * @param norm Should the graph be normalised?
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre \f$s \neq t\f$
	 * @pre Edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	free_tree& add_edge
	(node s, node t, bool norm = true, bool check_norm = true) noexcept;

	/**
	 * @brief Adds an edge to the graph.
	 *
	 * This method only adds an edge, and does no other work: normalisation
	 * is not checked, and no extra work per edge is done.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @pre \f$u \neq v\f$.
	 * @pre The edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	free_tree& add_edge_bulk(node s, node t) noexcept;

	/**
	 * @brief Finishes adding edges in bulk.
	 * @param norm Normalise the tree.
	 * @param check Check whether the tree is normalised or not.
	 * @pre All edges of the have been added with method @ref add_edge_bulk.
	 */
	void finish_bulk_add(bool norm = true, bool check = true) noexcept;

	/**
	 * @brief Adds a list of edges to the graph.
	 *
	 * This function checks that edges will not produce cycles only in a
	 * @e debug compilation of the library. Moreover, this operation is
	 * faster than calling @ref add_edge since the edges are added in bulk.
	 * For a more controlled addition of the edges, see @ref can_add_edges.
	 *
	 * <b>For developers:</b> method @ref lal::graphs::graph::actions_after_add_edge is
	 * called after each edge has been added.
	 * @param edges The edges to be added.
	 * @param norm Normalise the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge.
	 * @pre None of the subsets of the list of edges can produce cycles
	 * when added.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edges.
	 */
	free_tree& add_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
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
	 * @param norm Normalise the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The graph has been initialized with as many nodes as vertices in the
	 * list of edges.
	 * @pre There are no repeated edges in the list.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The type of tree (@ref m_is_tree_type_valid) is invalidated.
	 */
	free_tree& set_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * <b>For developers:</b> method @ref lal::graphs::graph::actions_after_remove_edge is
	 * called after the edge has been removed.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The edge must exist.
	 * @post If @e norm is true the tree is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The type of tree (@ref m_is_tree_type_valid) is invalidated.
	 */
	free_tree& remove_edge
	(node s, node t, bool norm = false, bool check_norm = true) noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed in
	 * bulk.
	 *
	 * <b>For developers:</b> method @ref lal::graphs::graph::actions_after_remove_edge is
	 * called after each edge has been removed.
	 * @param edges The edges to be deleted.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge(node,node,bool,bool).
	 * @post If @e norm is true the tree is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The type of tree (@ref m_is_tree_type_valid) is invalidated.
	 */
	free_tree& remove_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove all edges incident to a given vertex.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed
	 * in bulk.
	 *
	 * <b>For developers:</b> method
	 * @ref lal::graphs::graph::actions_after_remove_edge is called after each
	 * edge has been removed.
	 * @param u The node whose incident vertices are to be removed.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @post If @e norm is true the tree is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The type of tree (@ref m_is_tree_type_valid) is invalidated.
	 */
	virtual free_tree& remove_edges_incident_to
	(node u, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Disjoint union of trees.
	 *
	 * Given a free tree, append it to the current tree. If the current is empty
	 * (i.e., size 0), then the new tree is simply copied into the new one.
	 *
	 * If the current tree is not empty, all the new nodes in @e t (appended to
	 * the current tree) are relabelled starting at @e n, the number of nodes of
	 * the current tree.
	 *
	 * @param t Input tree.
	 * @post If the current tree is empty, the whole state of the new tree is
	 * copied into the new one.
	 * @post If the current tree is not empty, then the resulting graph is not
	 * a tree since it lacks an edge. Then method @ref is_tree() returns false.
	 * @post The type of tree (@ref m_is_tree_type_valid) is invalidated.
	 */
	void disjoint_union(const free_tree& t) noexcept;

	void calculate_tree_type() noexcept;

	/* GETTERS */

	bool is_rooted() const noexcept { return false; }

	/**
	 * @brief Converts a free tree into a head vector.
	 *
	 * See @ref LAL_concepts__head_vector page for a definition of 'head vector'.
	 * @param r A fictional root to be used to calculate the head vector.
	 * @param arr Linear arrangement of the tree.
	 * @returns The head vector representation of this tree.
	 * @pre This tree is a valid free tree (see @ref is_tree).
	 */
	head_vector get_head_vector(node r = 0, const linear_arrangement& arr = {})
	const noexcept;

protected:
	/// Initialises memory of @ref free_tree, @ref undirected_graph and @ref graph classes.
	virtual void _init(uint64_t n) noexcept {
		undirected_graph::_init(n);
		tree::tree_only_init(n);
	}
	/// Clears the memory of @ref free_tree, @ref undirected_graph and
	/// @ref graph classes.
	virtual void _clear() noexcept {
		undirected_graph::_clear();
		tree::tree_only_clear();
	}

	void update_union_find_after_edge_add(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) noexcept;
	void update_union_find_after_edge_add(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) const noexcept;

	void update_union_find_after_edge_remove(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) noexcept;
	void update_union_find_after_edge_remove(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) const noexcept;

	void update_union_find_before_incident_edges_removed(
		node u,
		uint64_t * const root_of, uint64_t * const root_size
	) noexcept;
	void update_union_find_before_incident_edges_removed(
		node u,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept;

	/// Copies all members of this class and the parent class.
	void copy_full_free_tree(const free_tree& f) noexcept {
		// copy undirected_graph class
		copy_full_undirected_graph(f);

		// copy only tree's members
		tree_only_copy(f);

		// copy this class' members
	}
	/// Moves all members of this class and the parent class.
	void move_full_free_tree(free_tree&& f) noexcept {
		// move-assign undirected_graph class
		move_full_undirected_graph(std::forward<free_tree>(f));

		// move-assign only tree's members
		tree_only_move(std::forward<free_tree>(f));

		// move this class' members
	}

private:
	using undirected_graph::disjoint_union;
};

} // -- namespace graphs
} // -- namespace lal
