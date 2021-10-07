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
#include <vector>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Undirected graph class.
 *
 * Class implementing an undirected graph, using the adjacency list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint64_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool, bool) ).
 */
class undirected_graph : virtual public graph {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	undirected_graph() noexcept { }
	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	undirected_graph(uint64_t n) noexcept {
		init(n);
	}
	/**
	 * @brief Copy constructor.
	 * @param g Undirected graph.
	 */
	undirected_graph(const undirected_graph& g) noexcept : graph() {
		copy_full_undirected_graph(g);
	}

	/**
	 * @brief Move constructor.
	 * @param g Undirected graph.
	 */
	undirected_graph(undirected_graph&& g) noexcept {
		move_full_undirected_graph(std::move(g));
	}

	/// Destructor.
	virtual ~undirected_graph() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Copy assignment operator.
	 * @param g Undirected graph.
	 */
	undirected_graph& operator= (const undirected_graph& g) noexcept {
		copy_full_undirected_graph(g);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param g Undirected graph.
	 */
	undirected_graph& operator= (undirected_graph&& g) noexcept {
		move_full_undirected_graph(std::move(g));
		return *this;
	}

	/* MODIFIERS */

	/**
	 * @brief Adds an edge to the graph.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_add is
	 * called after the edge has been added.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Should the graph be normalised?
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre \f$u \neq v\f$. The edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual undirected_graph& add_edge
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
	undirected_graph& add_edge_bulk(node s, node t) noexcept;

	void finish_bulk_add(bool norm = true, bool check = true) noexcept;

	/**
	 * @brief Adds a list of edges to the graph.
	 *
	 * This operation is faster than calling @ref add_edge(node,node,bool,bool)
	 * since the edges are added in bulk.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_add is
	 * called for every edge added.
	 * @param edges The edges to be added.
	 * @param norm Normalise the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge(node,node,bool,bool).
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual undirected_graph& add_edges
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
	 */
	virtual undirected_graph& set_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_remove is
	 * called after the edge has been removed.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The edge must exist.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual undirected_graph& remove_edge
	(node s, node t, bool norm = false, bool check_norm = true) noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed in
	 * bulk.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_remove is
	 * called after each edge has been removed.
	 * @param edges The edges to be deleted.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge(node,node,bool,bool).
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual undirected_graph& remove_edges
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
	 * @ref lal::graphs::graph::extra_work_per_edge_remove is called after each
	 * edge has been removed.
	 * @param u The node whose incident vertices are to be removed.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual undirected_graph& remove_edges_incident_to
	(node u, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Disjoint union of graphs.
	 *
	 * Given a graph, append it to the current graph.
	 *
	 * All the nodes in @e g are relabelled starting at @e n,
	 * the number of nodes of the current graph.
	 * @param g Input graph.
	 * @post The graph is normalised only if it was normalised before
	 * the call and @e g is also normalised.
	 */
	void disjoint_union(const undirected_graph& g) noexcept;

	/* SETTERS */

	/* GETTERS */

	std::vector<edge_pair> get_Q() const noexcept;

	std::vector<edge> get_edges() const noexcept;

	/**
	 * @brief Returns the neighbourhood of node @e u.
	 * @param u Node.
	 * @returns The list of nodes adjacent to node @e u.
	 */
	const neighbourhood& get_neighbours(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_adjacency_list[u];
	}

	/**
	 * @brief Returns the number of neighbours of @e u.
	 * @param u Node to be queried.
	 * @returns The number of adjacent nodes.
	 */
	uint64_t get_degree(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_adjacency_list[u].size();
	}

	/// Returns true if the edge \f$\{u,v\}\f$ exists in the graph.
	bool has_edge(node u, node v) const noexcept;

	bool is_directed() const noexcept { return false; }
	bool is_undirected() const noexcept { return true; }

protected:
	/// Initialises memory of @ref undirected_graph and @ref graph classes.
	virtual void _init(uint64_t n) noexcept;
	/// Clears the memory of @ref undirected_graph and @ref graph classes.
	virtual void _clear() noexcept;

	/// Copies all members of this class and the parent class.
	void copy_full_undirected_graph(const undirected_graph& u) noexcept;
	/// Moves all members of this class and the parent class.
	void move_full_undirected_graph(undirected_graph&& u) noexcept;

private:
	/**
	 * @brief Removes a single edge.
	 * @param u First node of edge.
	 * @param v Second node of edge.
	 * @param out_u Out-neighbourhood of node @e u.
	 * @param in_v In-neighbourhood of node @e v.
	 */
	void remove_single_edge
	(node u, node v, neighbourhood& out_u, neighbourhood& in_v) noexcept;
};

} // -- namespace graphs
} // -- namespace lal
