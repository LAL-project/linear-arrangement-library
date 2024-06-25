/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
 * further data (see @ref rooted_tree::init_rooted(const lal::graphs::free_tree&, node, bool, bool)).
 * While these classes' internal memory can be initialized, it can also be cleared
 * using method @ref clear. Each class reimplements this method to carry this
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
	 * @post The graph is normalized. The number of edges is 0.
	 */
	virtual void clear() noexcept;

	/**
	 * @brief Normalizes the graph.
	 *
	 * Sorts this graph's adjacency list structure in increasing order.
	 *
	 * Besides expensive, this method may be unnecessary. Method
	 * @ref check_normalized() checks whether the graph is normalized or
	 * not; in case it is, using this method is completely unnecessary.
	 * @post Method @ref is_normalized evaluates to true.
	 */
	virtual void normalize() noexcept;

	/**
	 * @brief Checks if the graph is normalized.
	 *
	 * Checks, whether the graph's adjacency structure is normalized or not.
	 * In case it is, attribute @ref m_is_normalized is set to true, so method
	 * @ref is_normalized evaluates to true.
	 */
	virtual bool check_normalized() noexcept;

	/**
	 * @brief Completes the inner structure of the graph after adding a bulk of edges
	 *
	 * This is meant to be used after several calls to
	 * @ref undirected_graph::add_edge_bulk,
	 * @ref directed_graph::add_edge_bulk.
	 * @param norm Normalize the graph.
	 * @param check Check wether the graph is normalized or not.
	 */
	virtual void finish_bulk_add(bool norm = true, bool check = true) noexcept = 0;

	/**
	 * @brief Completes the inner structure of the graph after removing edges in bulk.
	 *
	 * This is meant to be used after several calls to
	 * @ref undirected_graph::remove_edge_bulk,
	 * @ref directed_graph::remove_edge_bulk.
	 * @param norm Normalize the graph.
	 * @param check Check wether the graph is normalized or not.
	 */
	virtual void finish_bulk_remove(bool norm = true, bool check = true) noexcept = 0;

	/* SETTERS */

	/// Sets whether this graph is normalized or not.
	void set_normalized(bool v = true) noexcept { m_is_normalized = v; }

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
	 * @brief Returns whether this graph is normalized or not.
	 *
	 * A graph is normalized if every node's adjacency list is sorted
	 * increasingly. For this, use method @ref normalize().
	 * @returns The value of @ref m_is_normalized.
	 */
	bool is_normalized() const noexcept { return m_is_normalized; }

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
	 * @brief Is this graph normalized?
	 *
	 * An undirected graph is normalized iff every node's adjacency list is
	 * sorted in increasing order.
	 *
	 * In directed graphs, however, it is necessary that the adjacency lists
	 * of the out-neighbors and in-neighbors of nodes be sorted.
	 *
	 * This attribute is set to 'true' in all graph's initialisation
	 * and destruction (when @ref clear() method is called).
	 */
	bool m_is_normalized = true;

protected:
	/**
	 * @brief Initializes memory of @ref graph class.
	 * @param n Number of nodes.
	 * @pre The graph is cleared.
	 */
	virtual void _init(uint64_t n) noexcept {
		m_num_edges = 0;
		m_is_normalized = true;
		m_adjacency_list.resize(n);
	}
	/// Clears memory for the @ref graph class.
	virtual void _clear() noexcept {
		m_num_edges = 0;
		m_is_normalized = true;
		m_adjacency_list.clear();
	}

	/// Copies all members of this class.
	void copy_full_graph(const graph& g) noexcept {
		m_adjacency_list = g.m_adjacency_list;
		m_num_edges = g.m_num_edges;
		m_is_normalized = g.m_is_normalized;
	}
	/// Moves all members of this class.
	void move_full_graph(graph&& g) noexcept {
		m_adjacency_list = std::move(g.m_adjacency_list);
		m_num_edges = std::move(g.m_num_edges);
		m_is_normalized = std::move(g.m_is_normalized);

		g.m_num_edges = 0;
		g.m_is_normalized = false;
	}

	/// Adds a node to the graph.
	void __add_node() noexcept {
		m_adjacency_list.emplace_back();
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
	 * @post The graph is normalized only if it was normalized before the call
	 * and @e g is also normalized.
	 */
	void __disjoint_union(const graph& g) noexcept;

	/**
	 * @brief Do some extra work after the addition of an edge.
	 * @param u Node of the edge
	 * @param v Node of the edge
	 */
	virtual void actions_after_add_edge(node u, node v) noexcept;

	/**
	 * @brief Do some extra work after the addition of several edges.
	 * @param e List of edges.
	 */
	virtual void actions_after_add_edges(const edge_list& e) noexcept;

	/**
	 * @brief Do some extra work after the addition of several edges in bulk.
	 *
	 * This method should only be called after several calls to
	 * @ref undirected_graph::add_edge_bulk or @ref directed_graph::add_edge_bulk.
	 */
	virtual void actions_after_add_edges_bulk() noexcept;

	/**
	 * @brief Do some extra work after the removal of an edge.
	 * @param u Node of the edge
	 * @param v Node of the edge
	 */
	virtual void actions_after_remove_edge(node u, node v) noexcept;

	/**
	 * @brief Do some extra work after the removal of several edges.
	 * @param e List of edges.
	 */
	virtual void actions_after_remove_edges(const edge_list& e) noexcept;

	/**
	 * @brief Do some extra work after the removal of several edges in bulk.
	 *
	 * This method should only be called after several calls to
	 * @ref undirected_graph::remove_edge_bulk or @ref directed_graph::remove_edge_bulk.
	 */
	virtual void actions_after_remove_edges_bulk() noexcept;

	/**
	 * @brief Do some work before all edges incident to a node is removed.
	 * @param u Node whose incident edges are to be removed.
	 */
	virtual void actions_before_remove_edges_incident_to(node u) noexcept;

	/**
	 * @brief Do some work before the removal of a vertex.
	 * @param u Node to be removed.
	 */
	virtual void actions_after_remove_node(node u) noexcept;

	/// Normalize the graph after one (or more) edges have been added
	void normalize_after_edge_addition(bool norm, bool check) noexcept;
	/// Normalize the graph after one (or more) edges have been removed
	void normalize_after_edge_removal(bool norm, bool check) noexcept;
};

} // -- namespace graphs
} // -- namespace lal
