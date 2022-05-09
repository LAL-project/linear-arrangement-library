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
#include <vector>

// lal includes
#include <lal/basic_types.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Abstract class for graphs.
 *
 * Class used as an interface for all types of graphs. This means that this
 * class cannot be instantiated. The classes that can be instantiated are
 * @ref undirected_graph, @ref directed_graph, @ref free_tree, @ref rooted_tree.
 *
 * A usual way of initialising classes inheriting from this one is to use one
 * of the @ref init methods available. Depending on the subclass, this method
 * admits either the number of nodes of the graph or a whole other graph and
 * further information (see
 * @ref rooted_tree::init_rooted(const free_tree&,node). While these classes'
 * internal memory can be initialised, it can also be cleared using method
 * @ref clear. Each class reimplements this method to carry this
 * task appropriately.
 */
class graph {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	graph() noexcept { }
	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */

	graph(uint64_t n) noexcept {
		_init(n);
	}

	/**
	 * @brief Copy constructor.
	 * @param g Graph.
	 */
	graph(const graph& g) noexcept {
		copy_full_graph(g);
	}

	/**
	 * @brief Move constructor.
	 * @param g Graph.
	 */
	graph(graph&& g) noexcept {
		move_full_graph(std::forward<graph>(g));
	}

	/// Destructor.
	virtual ~graph() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Copy assignment operator.
	 * @param g Graph.
	 */
	graph& operator= (const graph& g) noexcept {
		copy_full_graph(g);
		return *this;
	}

	/**
	 * @brief Move assignment operator.
	 * @param g Graph.
	 */
	graph& operator= (graph&& g) noexcept {
		move_full_graph(std::forward<graph>(g));
		return *this;
	}

	/* MODIFIERS */

	/**
	 * @brief Allocates the necessary memory for this class.
	 *
	 * See @ref _init for details.
	 * @param n Number of nodes.
	 */
	virtual void init(uint64_t n) noexcept;
	/**
	 * @brief Frees the memory occupied by this graph.
	 *
	 * See @ref _clear for details.
	 * @post The graph is normalised. The number of edges is 0.
	 */
	virtual void clear() noexcept;

	/**
	 * @brief Normalises the graph.
	 *
	 * Sorts this graph's adjacency list structure in increasing order.
	 *
	 * Besides expensive, this method may be unnecessary. Method
	 * @ref check_normalised() checks whether the graph is normalised or
	 * not; in case it is, using this method is completely unnecessary.
	 * @post Method @ref is_normalised evaluates to true.
	 */
	virtual void normalise() noexcept;

	/**
	 * @brief Checks if the graph is normalised.
	 *
	 * Checks, whether the graph's adjacency structure is normalised or not.
	 * In case it is, attribute @ref m_normalised is set to true, so method
	 * @ref is_normalised evaluates to true.
	 */
	virtual bool check_normalised() noexcept;

	/**
	 * @brief Completes the inner structure of the graph after adding a bulk of edges
	 *
	 * This is meant to be used after several calls to
	 * @ref undirected_graph::add_edge_bulk,
	 * @ref directed_graph::add_edge_bulk.
	 *
	 * Also, the graph must have been completely constructed after all the
	 * bulk additions of edges to the graph.
	 * @param norm Normalise the graph.
	 * @param check Check wether the graph is normalised or not.
	 */
	virtual void finish_bulk_add(bool norm = true, bool check = true) noexcept = 0;

	/* SETTERS */

	/// Sets whether this graph is normalised or not.
	void set_normalised(bool v = true) noexcept { m_normalised = v; }

	/* GETTERS */

	/**
	 * @brief Returns all independent pairs of edges of this graph.
	 *
	 * The set \f$Q(G)\f$ is defined as the pairs of edges of \f$G\f$,
	 * \f$E(G) \times E(G)\f$, that are independent, that is, that share
	 * no nodes.
	 */
	virtual std::vector<edge_pair> get_Q() const noexcept = 0;

	/// Returns true if node @e u is in this graph.
	bool has_node(node u) const noexcept { return u < get_num_nodes(); }

	/// Returns true if the undirected edge (@e u, @e v) exists in the graph.
	virtual bool has_edge(node u, node v) const = 0;

	/// Returns the number of ndoes.
	uint64_t get_num_nodes() const noexcept
	{ return m_adjacency_list.size(); }

	/// Returns the number of edges.
	uint64_t get_num_edges() const noexcept { return m_num_edges; }

	/// Returns all edges of this graph.
	virtual std::vector<edge> get_edges() const noexcept = 0;

	/**
	 * @brief Returns whether this graph is normalised or not.
	 *
	 * A graph is normalised if every node's adjacency list is sorted
	 * increasingly. For this, use method @ref normalise().
	 * @returns The value of @ref m_normalised.
	 */
	bool is_normalised() const noexcept { return m_normalised; }

	/// Returns whether this graph is directed or not.
	virtual bool is_directed() const noexcept = 0;
	/// Returns whether this graph is undirected or not.
	virtual bool is_undirected() const noexcept = 0;

protected:
	/// Data structure that implements the graph.
	std::vector<neighbourhood> m_adjacency_list;
	/// Amount of edges of this graph.
	uint64_t m_num_edges = 0;
	/**
	 * @brief Is this graph normalised?
	 *
	 * An undirected graph is normalised iff every node's adjacency list is
	 * sorted in increasing order.
	 *
	 * In directed graphs, however, it is necessary that the adjacency lists
	 * of the out-neighbours and in-neighbours of nodes be sorted.
	 *
	 * This attribute is set to 'true' in all graph's initialisation
	 * and destruction (when @ref clear() method is called).
	 */
	bool m_normalised = true;

protected:
	/// Initialises memory of @ref graph class.
	virtual void _init(uint64_t n) noexcept {
		m_num_edges = 0;
		m_normalised = true;
		m_adjacency_list = std::vector<neighbourhood>(n);
	}
	/// Clears memory for the @ref graph class.
	virtual void _clear() noexcept {
		m_num_edges = 0;
		m_normalised = true;
		m_adjacency_list.clear();
	}

	/// Copies all members of this class.
	void copy_full_graph(const graph& g) noexcept {
		m_adjacency_list = g.m_adjacency_list;
		m_num_edges = g.m_num_edges;
		m_normalised = g.m_normalised;
	}
	/// Moves all members of this class.
	void move_full_graph(graph&& g) noexcept {
		m_adjacency_list = std::move(g.m_adjacency_list);
		m_num_edges = std::move(g.m_num_edges);
		m_normalised = std::move(g.m_normalised);

		g.m_num_edges = 0;
		g.m_normalised = false;
	}

	/**
	 * @brief Disjoint union of graphs.
	 *
	 * Given a graph, append it to the current graph.
	 *
	 * All the nodes in @e g are relabelled starting at @e n, the number of
	 * nodes of the current graph.
	 * @param g Input graph.
	 * @pre This graph and @e g must be of the same type (both must be either
	 * undirected, or both directed).
	 * @post The graph is normalised only if it was normalised before the call
	 * and @e g is also normalised.
	 */
	void __disjoint_union(const graph& g) noexcept;

	/**
	 * @brief Do some work before an node is removed.
	 * @param u Node to be removed.
	 */
	virtual void actions_after_remove_node(node u) noexcept;

	/**
	 * @brief Do some work before all edges incident to a node is removed.
	 * @param u Node whose incident edges are to be removed.
	 */
	virtual void actions_before_remove_edges_incident_to(node u) noexcept;

	/**
	 * @brief Do some extra work after an edge has been added.
	 * @param u Node of the edge
	 * @param v Node of the edge
	 */
	virtual void actions_after_add_edge(node u, node v) noexcept;
	/**
	 * @brief Do some extra work after an edge has been removed.
	 * @param u Node of the edge
	 * @param v Node of the edge
	 */
	virtual void actions_after_remove_edge(node u, node v) noexcept;

	/// Normalise the graph after one (or more) edges have been added
	void normalise_after_edge_addition(bool norm, bool check) noexcept;
	/// Normalise the graph after one (or more) edges have been removed
	void normalise_after_edge_removal(bool norm, bool check) noexcept;
};

} // -- namespace graphs
} // -- namespace lal
