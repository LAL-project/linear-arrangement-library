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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>
#include <array>
#include <string>

// lal includes
#include <lal/graphs/graph.hpp>
#include <lal/graphs/tree_type.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Tree graph class.
 *
 * This is an abstract class for those tree-like graphs. Classes that implement
 * different abstractions of trees and that inherit from this class are:
 * @ref free_tree, @ref rooted_tree.
 *
 * In these classes the addition of edges is constrained so as to ensure that
 * the edges added actually yield trees, i.e., that cycles are never produced.
 * For the sake of efficiency, only debug compilations of the library
 * (compilations where the DEBUG symbol is defined) check that such additions
 * do not produce cycles. In case of doubt, one can query the class using methods
 * @ref can_add_edge or @ref can_add_edges prior to adding one or several edges.
 */
class tree : virtual public graph {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	tree() noexcept { }
	/**
	 * @brief Copy constructor.
	 * @param t Tree.
	 */
	tree(const tree& t) noexcept : graph() {
		tree_only_copy(t);
	}

	/**
	 * @brief Move constructor.
	 * @param t Tree.
	 */
	tree(tree&& t) noexcept {
		tree_only_move(std::move(t));
	}

	/// Destructor.
	virtual ~tree() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Copy assignment operator.
	 * @param t Tree.
	 */
	tree& operator= (const tree& t) noexcept {
		tree_only_copy(t);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param t Tree.
	 */
	tree& operator= (tree&& t) noexcept {
		tree_only_move(std::move(t));
		return *this;
	}

	/* MODIFIERS */

	/**
	 * @brief Calculates the type of tree.
	 *
	 * See @ref tree_type for the list of different tree types.
	 */
	virtual void calculate_tree_type() noexcept = 0;

	/**
	 * @brief Completes the inner structure of the tree after adding edges in bulk.
	 *
	 * This is meant to be used after several calls to
	 * @ref undirected_graph::add_edge_bulk,
	 * @ref directed_graph::add_edge_bulk.
	 *
	 * This method completes the Union-Find data structure and the other necessary
	 * members assuming that the tree is now complete.
	 * @param norm Normalize the graph.
	 * @param check Check wether the graph is normalized or not.
	 * @pre All edges have been added with method @ref undirected_graph::add_edge_bulk
	 * or @ref directed_graph::add_edge_bulk.
	 */
	virtual void finish_bulk_add_complete(bool norm = true, bool check = true) noexcept = 0;

	/**
	 * @brief Completes the inner structure of the tree after removing edges in bulk.
	 *
	 * This is meant to be used after several calls to
	 * @ref undirected_graph::remove_edge_bulk,
	 * @ref directed_graph::remove_edge_bulk.
	 *
	 * This method completes the Union-Find data structure and the other necessary
	 * members assuming that the tree is now empty.
	 * @param norm Normalize the graph.
	 * @param check Check wether the graph is normalized or not.
	 * @pre All edges have been added with method @ref undirected_graph::remove_edge_bulk
	 * or @ref directed_graph::remove_edge_bulk.
	 */
	virtual void finish_bulk_remove_complete(bool norm = true, bool check = true) noexcept = 0;

	/* GETTERS */

	/**
	 * @brief Is this graph an actual tree?
	 *
	 * Returns true if the number of edges is one less than the
	 * number of nodes.
	 *
	 * Note that this would not really be true if the addition of edges was not
	 * constrained. That is, since it is constrained (behind the scenes) in a
	 * way that no cycles can be produced (for example, see @ref free_tree::add_edge,
	 * or @ref free_tree::add_edges), then we only need to check for the number
	 * of edges.
	 *
	 * For further characterisations of a tree see \cite Harary1969a
	 * (chapter 4, pages 32-33).
	 * @returns True or false depending on whether this graph fits the defintion
	 * of tree.
	 */
	bool is_tree() const noexcept {
		// NOTE: this would not really be true if the addition of edges
		// was not constrained. Since it is, in a way that no cycles can
		// be produced, then we only need to check for the number of edges.
		return (get_num_nodes() == 0 ? true : get_num_edges() == get_num_nodes() - 1);

		// NOTE 2: this is only true in a debug compilation of the library
		// since a release compilation does not actually constrain the addition
		// of edges
	}

	/// Returns whether this tree is a rooted tree.
	virtual bool is_rooted() const noexcept = 0;

	/**
	 * @brief Can this edge be added?
	 *
	 * In a tree, an edge can only be added if it does not produce cycles,
	 * and it has not been added before.
	 *
	 * In a rooted tree, an edge can only be added if the in-degree of vertex
	 * @e t (see @ref directed_graph::get_in_degree) is exactly 1 after adding
	 * the edge.
	 * @param s First node of the edge.
	 * @param t Second node of the edge.
	 * @returns Whether or not this edge can be added to the tree without
	 * producing cycles.
	 */
	virtual bool can_add_edge(node s, node t) const noexcept;

	/**
	 * @brief Can these edges be added?
	 *
	 * In a tree, a set of edges can only be added if their addition to the
	 * tree do not produce cycles and none of them have been added before.
	 *
	 * In a rooted tree, edges \f$(s,t)\f$ can only be added if the in-degree of
	 * vertex @e t (see @ref directed_graph::get_in_degree) is exactly 1 after
	 * adding the edge.
	 * @param edges List of edges.
	 * @returns Whether or not these edges can be added to the tree without
	 * producing cycles.
	 */
	virtual bool can_add_edges(const std::vector<edge>& edges) const noexcept;

	/**
	 * @brief Representative node of the connected component in which @e u belongs.
	 *
	 * If the current graph lacks some edges then it is clearly a forest,
	 * i.e., a series of disconnected components. This function returns a
	 * representative node of the connected component to which node @e u belongs.
	 *
	 * Further, let \f$cc(u)\f$ be the connected component of vertex @e u, and
	 * \f$rep(cc(u))\f$ be the representative node of \f$cc(u)\f$. For every
	 * other node \f$v\in cc(u)\f$, this function will return the same
	 * representative node \f$rep(cc(u))\f$. Therefore, \f$rep(cc(u))=rep(cc(v))\f$
	 * for every \f$v\in cc(u)\f$.
	 * @param u Input node.
	 * @returns The representative node of node @e u's component.
	 */
	uint64_t get_component_representative(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_union_find__root_of[u];
	}

	/**
	 * @brief Checks if two nodes are in the same connected component.
	 * @param u First node.
	 * @param v Second node.
	 * @returns True or false.
	 */
	bool are_nodes_in_same_component(node u, node v) const noexcept {
		return get_component_representative(u) == get_component_representative(v);
	}

	/**
	 * @brief Amount of nodes in a connected component of the tree.
	 *
	 * If the current graph lacks some edges then it is clearly a forest,
	 * i.e., a series of disconnected components. This function returns the size
	 * of the component node @e u belongs to.
	 *
	 * In rooted trees one has to see this amount as the number of nodes of the
	 * component in the underlying undirected forest.
	 * @param u Input node.
	 * @returns The size of the connected component of @e u.
	 */
	uint64_t get_num_nodes_component(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_union_find__root_size[m_union_find__root_of[u]];
	}

	/**
	 * @brief Returns whether this tree is of type @e tt.
	 *
	 * See method @ref calculate_tree_type to know how to calculate a tree's type.
	 * @param tt Type of tree (see @ref graphs::tree_type).
	 * @returns True if this tree is of type @e tt.
	 */
	bool is_of_tree_type(const tree_type& tt) const noexcept {
		return m_tree_type[static_cast<std::size_t>(tt)];
	}

	/**
	 * @brief Is the type of this tree valid?
	 *
	 * This function enables users determine when this tree's type should be
	 * calculated.
	 *
	 * In case this function returns false, users should call function
	 * @ref calculate_tree_type in order to obtain a valid tree type. Note,
	 * however, that prior to calling the function the type of this tree might
	 * be @ref graphs::tree_type::unknown and that the tree type may remain
	 * @ref graphs::tree_type::unknown even after the type has been calculated.
	 * Nevertheless, users should be suspicious of a tree being of
	 * @ref graphs::tree_type::unknown (in fact, of any) type if this method
	 * returns false, yet they should be sure of it if the type was calculated
	 * via method @ref calculate_tree_type.
	 * @returns True or false depending on whether the tree type was calculated
	 * or not.
	 */
	bool is_tree_type_valid() const noexcept { return m_is_tree_type_valid; }

	/**
	 * @brief Returns the list of types as a list of strings.
	 * @returns The list of types as a list of strings.
	 */
	std::vector<std::string> get_tree_type_list() const noexcept;

protected:
	/// The root of every vertex in the union-find data structure
	std::vector<node> m_union_find__root_of;
	/**
	 * @brief The size of the connected component that a @e root belongs to.
	 *
	 * Formally, @e m_size_of[v] is the size of the connected component of
	 * a @e root vertex @e v. A vertex @e u is a root vertex if there
	 * exists a vertex @e w such that @ref m_union_find__root_of[w] = u.
	 *
	 * In this context, root is within the union-find data structure.
	 */
	std::vector<uint64_t> m_union_find__root_size;

	/// The type of this tree
	std::array<bool,__tree_type_size> m_tree_type;
	/**
	 * @brief Is the type of this tree valid?
	 *
	 * This attribute keeps track of whether or not the function
	 * @ref calculate_tree_type should be called before querying the type of
	 * this tree via function @ref is_of_tree_type.
	 */
	bool m_is_tree_type_valid = false;

protected:
	/**
	 * @brief Initializes only the memory of class @ref tree.
	 * @param n Number of vertices.
	 */
	void tree_only_init(uint64_t n) noexcept {
		m_union_find__root_of = std::vector<uint64_t>(n);
		m_union_find__root_size = std::vector<uint64_t>(n);
		for (node u = 0; u < n; ++u) {
			m_union_find__root_of[u] = u;
			m_union_find__root_size[u] = 1;
		}
		std::fill(m_tree_type.begin(), m_tree_type.end() - 1, false);
		tree_only_invalidate();
	}
	/// Clears the memory used by only class @ref tree.
	void tree_only_clear() noexcept {
		m_union_find__root_of.clear();
		m_union_find__root_size.clear();
		std::fill(m_tree_type.begin(), m_tree_type.end() - 1, false);
		tree_only_invalidate();
	}

	/// Copies only members of class @ref tree.
	void tree_only_copy(const tree& t) noexcept {
		// copy this class' members
		m_union_find__root_of = t.m_union_find__root_of;
		m_union_find__root_size = t.m_union_find__root_size;
		m_is_tree_type_valid = t.m_is_tree_type_valid;
		m_tree_type = t.m_tree_type;
	}
	/// Moves only members of class @ref tree.
	void tree_only_move(tree&& t) noexcept {
		// move this class' members
		m_union_find__root_of = std::move(t.m_union_find__root_of);
		m_union_find__root_size = std::move(t.m_union_find__root_size);
		m_is_tree_type_valid = t.m_is_tree_type_valid;
		m_tree_type = std::move(t.m_tree_type);

		t.tree_only_invalidate();
	}

	/**
	 * @brief Adds a node to this tree.
	 *
	 * Updates all the internal data structures.
	 */
	void tree_only_add_node() noexcept {
		const node new_node = m_union_find__root_of.size();
		m_union_find__root_of.push_back(new_node);
		m_union_find__root_size.push_back(1);
		tree_only_invalidate();
	}

	/**
	 * @brief Invalidates the aggregated information of the tree.
	 *
	 * Invalidates:
	 * - @ref m_is_tree_type_valid.
	 */
	void tree_only_invalidate() noexcept {
		m_is_tree_type_valid = false;
	}

	/**
	 * @brief Do some work after the addition of an edge.
	 * @param u First node of the edge.
	 * @param v Second node of the edge.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_add_edge(node u, node v) noexcept;

	/**
	 * @brief Do some work after the addition of several edges.
	 * @param e List of edges.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_add_edges(const edge_list& e) noexcept;

	/**
	 * @brief Do some work after the addition of several edges in bulk.
	 *
	 * To be called only after veral calls to @ref undirected_graph::add_edge_bulk
	 * or @ref directed_graph::add_edge_bulk.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_add_edges_bulk() noexcept;

	/**
	 * @brief Do some work after the addition of several edges in bulk.
	 *
	 * To be called only after veral calls to @ref undirected_graph::add_edge_bulk
	 * or @ref directed_graph::add_edge_bulk.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_add_edges_bulk_complete() noexcept;

	/**
	 * @brief Do some work after the removal of several edges in bulk.
	 *
	 * To be called only after veral calls to @ref undirected_graph::remove_edge_bulk
	 * or @ref directed_graph::remove_edge_bulk.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_remove_edges_bulk() noexcept;

	/**
	 * @brief Do some work after the removal of several edges in bulk.
	 *
	 * To be called only after veral calls to @ref undirected_graph::remove_edge_bulk
	 * or @ref directed_graph::remove_edge_bulk.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_remove_edges_bulk_complete() noexcept;

	/**
	 * @brief Do some work after the removal of an edge.
	 * @param u First node of the edge.
	 * @param v Second node of the edge.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_remove_edge(node u, node v) noexcept;

	/**
	 * @brief Do some work after the removal of several edges.
	 * @param e List of edges.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_remove_edges(const edge_list& e) noexcept;

	/**
	 * @brief Do some work before the removal of a vertex.
	 * @param u Node to be removed.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_after_remove_node(node u) noexcept;

	/**
	 * @brief Do some work before the removal of all edges incident to a vertex.
	 * @param u Node whose incident edges are to be removed.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_actions_before_remove_edges_incident_to(node u) noexcept;

	/**
	 * @brief Updates the data structures of a tree after the graph structure
	 * has had its set of edges set.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_set_edges() noexcept;

	/**
	 * @brief Removes a vertex from the union-find data structure.
	 * @param u Node that was removed.
	 * @post The tree type is invalidated.
	 * @post Updated union-find.
	 */
	void tree_only_remove_node(node u) noexcept;

	/// Fills the Union-Find data structure assuming that the graph
	/// structure has all of its edges.
	void fill_union_find() noexcept {
		const uint64_t n = get_num_nodes();
		for (node u = 0; u < n; ++u) {
			// all vertices point to root zero
			m_union_find__root_of[u] = 0;
		}
		// the size of the connected component of the root 0 is n
		m_union_find__root_size[0] = n;
	}

	/// Empties the Union-Find data structure assuming that the tree has no edges.
	void empty_union_find() noexcept {
		for (node u = 0; u < get_num_nodes(); ++u) {
			// all vertices point to root zero
			m_union_find__root_of[u] = u;
			m_union_find__root_size[u] = 1;
		}
	}

	/**
	 * @brief Update the union find data structure after an edge addition.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_after_add_edge
	 * which updates the Union-Find data structure under addition of an edge.
	 * @param u Node that is connected to @e v.
	 * @param v Node that is connected to @e u.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_after_add_edge(
		node u, node v,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;

	/**
	 * @brief Update the union find data structure after the addition of a set of edges.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_after_add_edges
	 * which updates the Union-Find data structure under addition of an edge.
	 * @param edges A set of edges.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_after_add_edges(
		const edge_list& edges,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;

	/**
	 * @brief Update the union find data structure after the addition of several edges.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_after_add_rem_edges_bulk
	 * which updates the Union-Find data structure after addition of several edges.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_after_add_edges_bulk
	(uint64_t * const root_of, uint64_t * const root_size)
	const noexcept = 0;

	/**
	 * @brief Update the union find data structure after the removal of several edges.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_after_add_rem_edges_bulk
	 * which updates the Union-Find data structure under removal of several edges.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_after_remove_edges_bulk
	(uint64_t * const root_of, uint64_t * const root_size)
	const noexcept = 0;

	/**
	 * @brief Update the union find data structure after an edge removal.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_after_remove_edge
	 * which updates the Union-Find data structure under removal of an edge.
	 * @param u Node that is connected to @e v.
	 * @param v Node that is connected to @e u.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_after_remove_edge(
		node u, node v,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;

	/**
	 * @brief Update the union find data structure after the removal of a set of edges.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_after_remove_edges
	 * which updates the Union-Find data structure under removal of an edge.
	 * @param edges A set of edges.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_after_remove_edges(
		const edge_list& edges,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;

	/**
	 * @brief Update the union find data structure before the removal of all
	 * edges incident to a node.
	 *
	 * This is a helper method to be able to call @ref lal::detail::update_unionfind_before_remove_edges_incident_to
	 * which updates the Union-Find data structure under removal of all incident
	 * edges to a node.
	 * @param u Node whose incident edges are to be removed.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void update_union_find_before_remove_incident_edges_to(
		node u,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;
};

} // -- namespace graphs
} // -- namespace lal
