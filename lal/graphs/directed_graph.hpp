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
#include <lal/graphs/undirected_graph.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Directed graph class.
 *
 * Class implementing a directed graph, using the adjacency list data structure.
 *
 * An object of this class must be initialised either with its constructor
 * or with the @ref init(uint32_t) method. Edges can then be added one by one
 * (see @ref add_edge(node,node,bool,bool) ) or all at the same time (see
 * @ref add_edges(const std::vector<edge>&, bool,bool) ).
 */
class directed_graph : virtual public graph {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	directed_graph() noexcept;
	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	directed_graph(uint32_t n) noexcept;
	/// Copy constructor.
	directed_graph(const directed_graph&) noexcept;
#ifndef SWIG
	/// Move constructor.
	directed_graph(directed_graph&&) noexcept;
#endif
	/// Destructor.
	virtual ~directed_graph() noexcept;

	/* OPERATORS */

#ifndef SWIG
	/// Copy assignment operator.
	directed_graph& operator= (const directed_graph&) noexcept;
	/// Move assignment operator.
	directed_graph& operator= (directed_graph&&) noexcept;
#endif

	/* MODIFIERS */

	void normalise() noexcept;

	bool check_normalised() noexcept;

	/**
	 * @brief Adds a directed edge to the graph.
	 *
	 * For developers: method @ref graph::extra_work_per_edge_add is
	 * called after the edge has been added.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Normalise the graph after the insertion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre \f$u \neq v\f$. The directed edge \f$(s,t)\f$ is not part of
	 * the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual directed_graph& add_edge
	(node s, node t, bool norm = false, bool check_norm = true) noexcept;

	/**
	 * @brief Adds an edge to the graph.
	 *
	 * This method only adds an edge, and does no other work: normalisation
	 * is not checked, and no extra work per edge is done.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @pre \f$u \neq v\f$. The edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	directed_graph& add_edge_bulk(node s, node t) noexcept;

	void finish_bulk_add(bool norm = true, bool check = true) noexcept;

	/**
	 * @brief Adds a list of directed edges to the graph.
	 *
	 * This operation is faster than adding edges one by one with
	 * @ref add_edge(node,node,bool,bool) since the edges are added in bulk.
	 *
	 * For developers: method @ref graph::extra_work_per_edge_add is
	 * called after each edge has been added.
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
	virtual directed_graph& add_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Sets the list of edges to the graph.
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
	 * @pre There are no repeated edges in the list.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual directed_graph& set_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * For developers: method @ref graph::extra_work_per_edge_remove is
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
	virtual directed_graph& remove_edge
	(node s, node t, bool norm = false, bool check_norm = true) noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed
	 * in bulk.
	 *
	 * For developers: method @ref graph::extra_work_per_edge_remove is
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
	virtual directed_graph& remove_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
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
	void disjoint_union(const directed_graph& g) noexcept;

	/* SETTERS */

	/* GETTERS */

	std::vector<edge_pair> Q() const noexcept;

	std::vector<edge> edges() const noexcept;

	/// Returns true if the edge \f$(u,v)\f$ exists in the graph.
	bool has_edge(node u, node v) const noexcept;

	/**
	 * @brief Returns the out-neighbours of node @e u
	 * @param u Node
	 * @return Returns the list of nodes leaving node @e u.
	 */
	inline const neighbourhood& get_out_neighbours(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_adjacency_list[u];
	}
	/**
	 * @brief Returns the in-neighbours of node @e u
	 * @param u Node
	 * @return Returns the list of nodes entering at node @e u.
	 */
	inline const neighbourhood& get_in_neighbours(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_in_adjacency_list[u];
	}

	/**
	 * @brief Returns the in-degree plus the out-degree of this vertex.
	 *
	 * Returns the degree of this vertex in its underlying undirected structure.
	 * Same as @ref in_degree + @ref out_degree.
	 * @param u Vertex
	 * @return Returns the (in + out) degree of this vertex.
	 */
	inline uint32_t degree(node u) const noexcept
	{ return out_degree(u) + in_degree(u); }

	/// Returns the out-degree of a node.
	inline uint32_t out_degree(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return static_cast<uint32_t>(m_adjacency_list[u].size());
	}
	/// Returns the in-degree of a node.
	inline uint32_t in_degree(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return static_cast<uint32_t>(m_in_adjacency_list[u].size());
	}

	inline bool is_directed() const noexcept { return true; }
	inline bool is_undirected() const noexcept { return false; }

	/**
	 * @brief Converts this directed graph into an undirected graph.
	 *
	 * The undirected graph returned connects two vertices \f$u,v\f$ if
	 * these two vertices are connected by a directed edge (\f$(u,v)\f$ or
	 * \f$(v,u)\f$) in this graph. In other words, if two vertices are
	 * connected by a single directed edge, the direction is dropped. If
	 * two edges are connected by two directed edges (of opposite directions)
	 * then the two are merged into a single undirected edge.
	 * @param norm Normalise the graph.
	 * @param check Chech whether the resulting graph is normalised or not.
	 * @return Returns this graph in which the edges are undirected.
	 */
	undirected_graph to_undirected
	(bool norm = true, bool check = true) const noexcept;

protected:
	/// In-neighbours for every node.
	std::vector<neighbourhood> m_in_adjacency_list;

protected:
	/// Initialises memory of @ref directed_graph and @ref graph classes.
	virtual void _init(uint32_t n) noexcept;
	/// Clears the memory of @ref directed_graph and @ref graph classes.
	virtual void _clear() noexcept;

	/// Copies all members of this class and the parent class.
	void copy_full_directed_graph(const directed_graph& d) noexcept;
	/// Moves all members of this class and the parent class.
	void move_full_directed_graph(directed_graph&& d) noexcept;

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
