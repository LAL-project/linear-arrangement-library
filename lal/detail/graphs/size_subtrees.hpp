/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2022
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
#include <cassert>
#endif
#include <functional>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/detail/type_traits/is_pointer_iterator.hpp>
#include <lal/detail/pairs_utils.hpp>

namespace lal {
namespace detail {

/**
 * @brief Calculate the size of every subtree of the tree @e t.
 *
 * @param t Input tree.
 * @param u Parent node (the first call should be an invalid value (e.g., n+1)).
 * @param v Next node in the exploration of the tree.
 * @param sizes The size of the subtree rooted at every reachable node
 * from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
void get_size_subtrees
(const tree_t& t, const node u, const node v, uint64_t * const sizes)
noexcept
{
	sizes[v] = 1;

	if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
		for (const node w : t.get_out_neighbours(v)) {
			if (w == u) { continue; }
			get_size_subtrees(t, v, w, sizes);
			sizes[v] += sizes[w];
		}
		for (const node w : t.get_in_neighbours(v)) {
			if (w == u) { continue; }
			get_size_subtrees(t, v, w, sizes);
			sizes[v] += sizes[w];
		}
	}
	else {
		for (const node w : t.get_neighbours(v)) {
			if (w == u) { continue; }
			get_size_subtrees(t, v, w, sizes);
			sizes[v] += sizes[w];
		}
	}
}

/**
 * @brief Calculate the size of every subtree of tree @e t.
 *
 * The method starts calculating the sizes at node @e r. Since rooted trees
 * have directed edges, starting at a node different from the tree's root
 * may not calculate every subtree's size.
 * @param t Input rooted tree.
 * @param r Start calculating sizes of subtrees at this node.
 * @param sizes The size of the subtree rooted at every reachable node from @e r.
 * @pre Parameter @e sizes has size the number of vertices.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
void get_size_subtrees(const tree_t& t, node r, uint64_t * const sizes)
noexcept
{
#if defined DEBUG
	assert(sizes != nullptr);
#endif
	get_size_subtrees(t, t.get_num_nodes(), r, sizes);
}

/**
 * @brief Calculates the values \f$s(u,v)\f$ for the edges \f$(s,t)\f$ reachable
 * from \f$v\f$ in the subtree \f$T^u_v\f$.
 *
 * This function calculates the 'map' relating each edge \f$(u, v)\f$ with the
 * size of the subtree rooted at \f$v\f$ with respect to the hypothetical root
 * \f$u\f$. This is an implementation of the algorithm described in
 * \cite Hochberg2003a (proof of lemma 8 (page 63), and the beginning of
 * section 6 (page 65)).
 *
 * Notice that the values are not stored in an actual map (std::map, or similar),
 * but in a vector.
 * @tparam iterated_t The type that stores the sizes.
 * @tparam iterator_t The type of the iterator on a container containing values
 * of type iterated_t.
 * @param t Input tree.
 * @param n Size of the connected component to which edge \f$(u,v)\f$ belongs to.
 * @param u First vertex of the edge.
 * @param v Second vertex of the edge.
 * @param it An iterator to the container that holds the size values. Such
 * container must have size equal to twice the number of edges in the connected
 * component of @e u and @e v.
 * @pre Vertices @e u and @e v belong to the same connected component.
 */
template <
	class tree_t,
	typename iterator_t,
	std::enable_if_t<
		std::is_base_of_v<graphs::tree, tree_t>
		and is_pointer_iterator_v<edge_size, iterator_t>,
		bool
	> = true
>
uint64_t calculate_bidirectional_sizes
(const tree_t& t, const uint64_t n, const node u, const node v, iterator_t& it)
noexcept
{
	uint64_t s = 1;
	if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
		for (const node w : t.get_out_neighbours(v)) {
			if (w == u) { continue; }
			s += calculate_bidirectional_sizes(t,n, v, w, it);
		}
		for (const node w : t.get_in_neighbours(v)) {
			if (w == u) { continue; }
			s += calculate_bidirectional_sizes(t,n, v, w, it);
		}
	}
	else {
		for (const node w : t.get_neighbours(v)) {
			if (w == u) { continue; }
			s += calculate_bidirectional_sizes(t,n, v, w, it);
		}
	}

	*it++ = {edge(u,v), s};
	*it++ = {edge(v,u), n - s};
	return s;
}

/**
 * @brief Calculates the values \f$s_u(v)\f$ for the edges \f$(u,v)\f$ reachable
 * from vertex @e x.
 *
 * Calculates the values \f$s_u(v)\f$ for all edges \f$(u,v)\f$ in linear time.
 * This is an implementation of the algorithm described in \cite Hochberg2003a
 * (proof of lemma 8 (page 63), and the beginning of section 6 (page 65)).
 *
 * For any edge \f$(u,v)\f$ let \f$T^u\f$ be the tree \f$T\f$ rooted at \f$u\f$.
 * The value \f$s_u(v)\f$ is the size of the subtree of \f$T^u\f$ rooted at \f$v\f$,
 * i.e., \f$s_u(v)=|V(T^u_v)|\f$.
 *
 * Example of usage (mind the vector! its initial size is \f$2*m\f$).
 *
 @code
 const free_tree t = ... ;
 vector<pair<edge,uint64_t>> sizes_edges(2*t.get_num_edges());
 auto it = sizes_edges.begin();
 detail::calculate_bidirectional_sizes(t, t.get_num_nodes(), 0, it);
 @endcode
 *
 * @tparam tree_t Type of the tree. Must be 'rooted_tree' or 'free_tree'.
 * @tparam iterated_t The type that stores the sizes.
 * @tparam iterator_t The type of the iterator on a container containing values
 * of type iterated_t.
 * @param t Input tree
 * @param n Number of nodes in the connected component of vertex @e x
 * @param x Node of the connected component for which we want to calculate the
 * bidirectional sizes
 * @param it An iterator to the container that holds the size values. Such
 * container must have size equal to twice the number of edges in the connected
 * component of @e u and @e v.
 */
template <
	class tree_t,
	typename iterator_t,
	std::enable_if_t<
		std::is_base_of_v<graphs::tree, tree_t>
		and is_pointer_iterator_v<edge_size, iterator_t>,
		bool
	> = true
>
void calculate_bidirectional_sizes
(const tree_t& t, const uint64_t n, const node x, iterator_t it)
noexcept
{
	if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
		for (const node y : t.get_out_neighbours(x)) {
			calculate_bidirectional_sizes(t,n, x, y, it);
		}
		for (const node y : t.get_in_neighbours(x)) {
			calculate_bidirectional_sizes(t,n, x, y, it);
		}
	}
	else {
		for (const node y : t.get_neighbours(x)) {
			calculate_bidirectional_sizes(t,n, x, y, it);
		}
	}
}

} // -- namespace detail
} // -- namespace lal
