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
#include <array>

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
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param t Tree.
	 */
	tree(tree&& t) noexcept {
		tree_only_move(std::move(t));
	}
#endif
	/// Destructor.
	virtual ~tree() noexcept { }

	/* OPERATORS */

#ifndef SWIG
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
#endif

	/* MODIFIERS */

	/**
	 * @brief Calculates the type of tree.
	 *
	 * See @ref tree_type for the list of different tree types.
	 */
	virtual void calculate_tree_type() noexcept = 0;

	/* GETTERS */

	/**
	 * @brief Is this graph is an actual tree?
	 *
	 * Returns true if the number of edges is one less than the
	 * number of nodes. Note that this would not really be true if the
	 * addition of edges was not constrained. Since it is constrained in a
	 * way that no cycles can be produced (for example, see
	 * @ref free_tree::add_edge, or @ref free_tree::add_edges), then we
	 * only need to check for the number of edges.
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
	 * @param s First node of the edge.
	 * @param t Second node of the edge.
	 * @returns Whether the addition of this new edge can be added
	 * to the tree without producing cycles.
	 */
	bool can_add_edge(node s, node t) const noexcept;

	/**
	 * @brief Can these edges be added?
	 *
	 * In a tree, a set of edges can only be added if their addition to the
	 * tree do not produce cycles and none of them have been added before.
	 * @param edges List of edges.
	 * @returns Whether the addition of these new edges can be added
	 * to the tree without producing cycles.
	 */
	bool can_add_edges(const std::vector<edge>& edges) const noexcept;

	/**
	 * @brief Amount of nodes in a connected component of the tree.
	 *
	 * When tree has had an edge removed, or when it is not completely
	 * built, i.e., it lack some edges, the resulting graph is clearly
	 * a forest. This function returns the size of the forest node @e u
	 * belongs to.
	 *
	 * In directed trees one has to see this amount as the number of nodes
	 * of the component in the undirected version of the forest.
	 * @param u Input node.
	 * @returns The size of the connected component of @e u.
	 */
	inline uint64_t get_num_nodes_component(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_root_size[m_root_of[u]];
	}

	/**
	 * @brief Returns whether this tree is of type @e tt.
	 *
	 * See method @ref calculate_tree_type to know how to calculate a tree's type.
	 * @param tt Type of tree (see @ref lal::graphs::tree_type).
	 * @returns True if this tree is of type @e tt.
	 */
	inline bool is_of_tree_type(const tree_type& tt) const noexcept {
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
	 * be @ref lal::graphs::tree_type::unknown and that the tree type may remain
	 * @ref lal::graphs::tree_type::unknown even after the type has been calculated.
	 * Nevertheless, users should be suspicious of a tree being of
	 * @ref lal::graphs::tree_type::unknown (in fact, of any) type if this method
	 * returns false, yet they should be sure of it if the type was calculated
	 * via method @ref calculate_tree_type.
	 * @returns True or false depending on whether the tree type was calculated
	 * or not.
	 */
	inline bool is_tree_type_valid() const noexcept { return m_is_tree_type_valid; }

	/**
	 * @brief Returns the list of types as a list of strings.
	 * @returns The list of types as a list of strings.
	 */
	std::vector<std::string> get_tree_type_list() const noexcept;

protected:
	/// The root of every vertex in the union-find data structure
	std::vector<node> m_root_of;
	/**
	 * @brief The size of the connected component that a @e root belongs to.
	 *
	 * Formally, @e m_size_of[v] is the size of the connected component of
	 * a @e root vertex @e v. A vertex @e u is a root vertex if there
	 * exists a vertex @e w such that @ref m_root_of[w] = u.
	 *
	 * In this context, root is within the union-find data structure.
	 */
	std::vector<uint64_t> m_root_size;

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
	 * @brief Initialises only the memory of class @ref tree.
	 * @param n Number of vertices.
	 */
	void tree_only_init(uint64_t n) noexcept;
	/// Clears the memory used by only class @ref tree.
	void tree_only_clear() noexcept;

	/// Copies only members of class @ref tree.
	void tree_only_copy(const tree& t) noexcept;
	/// Moves only members of class @ref tree.
	void tree_only_move(tree&& t) noexcept;

	void extra_work_per_edge_add(node u, node v) noexcept;
	void extra_work_per_edge_remove(node u, node v) noexcept;

	/// Updates the data structures of a tree after the graph structure
	/// has had its set of edges set.
	void tree_only_extra_work_edges_set() noexcept;

	/// Fills the Union-Find data structure assuming that the graph
	/// structure has all of its edges.
	void fill_union_find() noexcept;

	/**
	 * @brief A call to the union find method.
	 *
	 * This is a helper method to be able to call a template in the
	 * lal::internal namespace which updates the union find data structure
	 * under addition of an edge.
	 * @param u Node that is connected to @e v.
	 * @param v Node that is connected to @e u.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void call_union_find_after_add(
		node u, node v,
		uint64_t * const root_of, uint64_t * const root_size
	) noexcept = 0;
	/**
	 * @brief A const call to the union find method.
	 *
	 * This is a helper method to be able to call a template in the
	 * lal::internal namespace which updates the union find data structure
	 * under addition of an edge.
	 * @param u Node that is connected to @e v.
	 * @param v Node that is connected to @e u.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void call_union_find_after_add(
		node u, node v,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;
	/**
	 * @brief A call to the union find method.
	 *
	 * This is a helper method to be able to call a template in the
	 * lal::internal namespace which updates the union find data structure
	 * under removal of an edge.
	 * @param u Node that is connected to @e v.
	 * @param v Node that is connected to @e u.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void call_union_find_after_remove(
		node u, node v,
		uint64_t * const root_of, uint64_t * const root_size
	) noexcept = 0;
	/**
	 * @brief A const call to the union find method.
	 *
	 * This is a helper method to be able to call a template in the
	 * lal::internal namespace which updates the union find data structure
	 * under removal of an edge.
	 * @param u Node that is connected to @e v.
	 * @param v Node that is connected to @e u.
	 * @param root_of Array of @e n elements relating each vertex to its root
	 * in the union find data structure.
	 * @param root_size Array of @e n elements relating each vertex to the size
	 * of the connected component it belongs to.
	 */
	virtual void call_union_find_after_remove(
		node u, node v,
		uint64_t * const root_of, uint64_t * const root_size
	) const noexcept = 0;
};

} // -- namespace graphs
} // -- namespace lal
