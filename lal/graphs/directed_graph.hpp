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
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/graph.hpp>
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Directed graph class.
 *
 * Class implementing a directed graph, using the adjacency list data structure.
 *
 * An object of this class must be initialized either with its constructor
 * or with the @ref init method. Edges can then be added one by one
 * (see @ref add_edge) or all at the same time (see @ref add_edges).
 */
class directed_graph : virtual public graph {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	directed_graph() noexcept : graph() { }
	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	directed_graph(const uint64_t n) noexcept {
		init(n);
	}
	/**
	 * @brief Copy constructor.
	 * @param g Directed graph.
	 */
	directed_graph(const directed_graph& g) noexcept : graph() {
		copy_full_directed_graph(g);
	}

	/**
	 * @brief Move constructor.
	 * @param g Directed graph.
	 */
	directed_graph(directed_graph&& g) noexcept {
		move_full_directed_graph(std::forward<directed_graph>(g));
	}

	/// Destructor.
	virtual ~directed_graph() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Copy assignment operator.
	 * @param g Directed graph.
	 */
	directed_graph& operator= (const directed_graph& g) noexcept {
		copy_full_directed_graph(g);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param g Directed graph.
	 */
	directed_graph& operator= (directed_graph&& g) noexcept {
		move_full_directed_graph(std::forward<directed_graph>(g));
		return *this;
	}

	/* MODIFIERS */

	/**
	 * @brief Predicts that the in-degree of node @e u is @e d.
	 *
	 * Memory of size @e d is reserved so that adding edges is done more efficiently.
	 * @param u Node to reserve the degree for.
	 * @param d The amount of memory to reserve.
	 * @pre The graph must have been initialized.
	 */
	void reserve_in_degree(const node u, const uint64_t d) noexcept {
#if defined DEBUG
		assert(u < get_num_nodes());
#endif
		m_in_adjacency_list[u].reserve(d);
	}
	/**
	 * @brief Predicts that the out-degree of node @e u is @e d.
	 *
	 * Memory of size @e d is reserved so that adding edges is done more efficiently.
	 * @param u Node to reserve the degree for.
	 * @param d The amount of memory to reserve.
	 * @pre The graph must have been initialized.
	 */
	void reserve_out_degree(const node u, const uint64_t d) noexcept {
#if defined DEBUG
		assert(u < get_num_nodes());
#endif
		m_adjacency_list[u].reserve(d);
	}

	void normalize() noexcept;

	[[nodiscard]] bool check_normalized() noexcept;

	/// Adds a node to the graph.
	virtual directed_graph& add_node() noexcept {
		graph::__add_node();
		m_in_adjacency_list.emplace_back();
		return *this;
	}

	/**
	 * @brief Remove a node from this graph.
	 *
	 * @param u Valid node index: \f$0 \le u < n\f$.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The node must exist.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	virtual directed_graph& remove_node
	(
		const node u,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Adds a directed edge to the graph.
	 *
	 * Method @ref actions_after_add_edge is called after the edge has been added.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Normalize the graph after the insertion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre \f$u \neq v\f$. The directed edge \f$(s,t)\f$ is not part of
	 * the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	virtual directed_graph& add_edge
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
	directed_graph& add_edge_bulk(const node s, const node t) noexcept;
	
	void finish_bulk_add(const bool norm = true, const bool check = true) noexcept;

	/**
	 * @brief Adds a list of directed edges to the graph.
	 *
	 * This operation is faster than adding edges one by one with
	 * @ref add_edge since the edges are added in bulk.
	 * @param edges The edges to be added.
	 * @param norm Normalize the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge(node,node,bool,bool).
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	virtual directed_graph& add_edges
	(
		const std::vector<edge>& edges,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Sets the list of edges to the graph.
	 *
	 * Sets the edges of this graph assuming that the nodes indexed in the
	 * list are, at most, the number of nodes of this graph.
	 *
	 * This list of edges is assumed to be all the edges that are going
	 * to be added to this graph. This means that the internal data structures
	 * are constructed more efficiently than when adding edges one by one
	 * (see @ref add_edge) or in several chunks (see @ref add_edges).
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
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	virtual directed_graph& set_edges
	(
		const std::vector<edge>& edges,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Removes an edge from the graph.
	 *
	 * This method only removes an edge, and does no other work: normalisation
	 * is not checked, and no extra work per edge is done.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @pre \f$u \neq v\f$.
	 * @pre The edge \f$\{s,t\}\f$ is part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the removal of the edge.
	 */
	virtual directed_graph& remove_edge_bulk(const node s, const node t) noexcept;

	void finish_bulk_remove(const bool norm = true, const bool check = true) noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * Method @ref actions_after_remove_edge is called after the edge has been
	 * removed.
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
	virtual directed_graph& remove_edge
	(
		const node s,
		const node t,
		const bool norm = true,
		const bool check_norm = true
	)
	noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge since the edges are removed in bulk.
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
	virtual directed_graph& remove_edges
	(const std::vector<edge>& edges, const bool norm = true, const bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove all edges incident to a given vertex.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge since the edges are removed in bulk.
	 *
	 * Method @ref actions_after_remove_edge is called after each edge has been
	 * removed.
	 * @param u The node whose incident vertices are to be removed.
	 * @param norm Normalize the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalized or not? This might be useful in case the
	 * resulting graph is normalized. If @e norm is true then @e check_norm
	 * is ignored.
	 * @post If @e norm is true the graph is guaranteed to be normalized
	 * after the addition of the edge.
	 */
	virtual directed_graph& remove_edges_incident_to
	(const node u, const bool norm = true, const bool check_norm = true)
	noexcept;

	/**
	 * @brief Disjoint union of graphs.
	 *
	 * Given a graph, append it to the current graph.
	 *
	 * All the nodes in @e g are relabelled starting at @e n,
	 * the number of nodes of the current graph.
	 * @param g Input graph.
	 * @post The graph is normalized only if it was normalized before
	 * the call and @e g is also normalized.
	 */
	directed_graph& disjoint_union(const directed_graph& g) noexcept;

	/* SETTERS */

	/* GETTERS */

	[[nodiscard]] std::vector<edge_pair> get_Q() const noexcept;

	[[nodiscard]] std::vector<edge> get_edges() const noexcept;

	/// Returns true if the edge \f$(u,v)\f$ exists in the graph.
	[[nodiscard]] bool has_edge(const node u, const node v) const noexcept;

	/**
	 * @brief Returns the out-neighbors of node @e u
	 * @param u Node
	 * @returns The list of nodes leaving node @e u.
	 */
	[[nodiscard]] const neighbourhood& get_out_neighbors(const node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_adjacency_list[u];
	}
	/**
	 * @brief Returns the in-neighbors of node @e u
	 * @param u Node
	 * @returns The list of nodes entering at node @e u.
	 */
	[[nodiscard]] const neighbourhood& get_in_neighbors(const node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_in_adjacency_list[u];
	}

	/**
	 * @brief Returns the in-degree plus the out-degree of this vertex.
	 *
	 * Returns the degree of this vertex in its underlying undirected structure.
	 * Same as @ref get_in_degree + @ref get_out_degree.
	 * @param u Vertex
	 * @returns The (in + out) degree of this vertex.
	 */
	[[nodiscard]] uint64_t get_degree(const node u) const noexcept
	{ return get_out_degree(u) + get_in_degree(u); }

	/// Returns the out-degree of a node.
	[[nodiscard]] uint64_t get_out_degree(const node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_adjacency_list[u].size();
	}
	/// Returns the in-degree of a node.
	[[nodiscard]] uint64_t get_in_degree(const node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_in_adjacency_list[u].size();
	}

	[[nodiscard]] bool is_directed() const noexcept { return true; }
	[[nodiscard]] bool is_undirected() const noexcept { return false; }

	/**
	 * @brief Converts this directed graph into an undirected graph.
	 *
	 * The undirected graph returned connects two vertices \f$u,v\f$ if
	 * these two vertices are connected by a directed edge (\f$(u,v)\f$ or
	 * \f$(v,u)\f$) in this graph. In other words, if two vertices are
	 * connected by a single directed edge, the direction is dropped. If
	 * two edges are connected by two directed edges (of opposite directions)
	 * then the two are merged into a single undirected edge.
	 * @param norm Normalize the graph.
	 * @param check Chech whether the resulting graph is normalized or not.
	 * @returns This graph in which the edges are undirected.
	 */
	[[nodiscard]] undirected_graph to_undirected
	(const bool norm = true, const bool check = true)
	const noexcept;

	/// Returns all the connected components of this graph as individual graphs.
	[[nodiscard]] std::vector<directed_graph> get_connected_components() const noexcept;

protected:
	/// In-neighbors for every node.
	std::vector<neighbourhood> m_in_adjacency_list;

protected:
	/**
	 * @brief Initializes the memory in the graph hierarchy.
	 *
	 * Initializes memory of @ref lal::graphs::directed_graph and
	 * @ref lal::graphs::graph classes.
	 * @param n Number of nodes.
	 * @pre The graph is cleared.
	 */
	virtual void _init(const uint64_t n) noexcept {
		graph::_init(n);
		m_in_adjacency_list.resize(n);
	}
	/// Clears the memory of @ref directed_graph and @ref graph classes.
	virtual void _clear() noexcept {
		graph::_clear();
		m_in_adjacency_list.clear();
	}

	virtual void actions_after_add_edge(const node u, const node v) noexcept;

	virtual void actions_after_add_edges(const edge_list& e) noexcept;

	virtual void actions_after_add_edges_bulk() noexcept;

	virtual void actions_after_remove_edge(const node u, const node v) noexcept;

	virtual void actions_after_remove_edges(const edge_list& e) noexcept;

	virtual void actions_after_remove_edges_bulk() noexcept;

	virtual void actions_before_remove_edges_incident_to(const node u) noexcept;

	virtual void actions_after_remove_node(const node u) noexcept;

	/// Copies all members of this class and the parent class.
	void copy_full_directed_graph(const directed_graph& d) noexcept {
		// copy parent class
		copy_full_graph(d);

		// copy this class' members
		m_in_adjacency_list = d.m_in_adjacency_list;
	}
	/// Moves all members of this class and the parent class.
	void move_full_directed_graph(directed_graph&& d) noexcept {
		// move-assign parent class
		move_full_graph(std::forward<directed_graph>(d));

		// move-assign this class' members
		m_in_adjacency_list = std::move(d.m_in_adjacency_list);
	}

private:
	/**
	 * @brief Removes a single edge.
	 * @param u First node of edge.
	 * @param v Second node of edge.
	 * @param out_u Out-neighbourhood of node @e u.
	 * @param in_v In-neighbourhood of node @e v.
	 */
	void remove_single_edge
	(
		const node u,
		const node v,
		neighbourhood& out_u,
		neighbourhood& in_v
	)
	noexcept;
};

} // -- namespace graphs
} // -- namespace lal
