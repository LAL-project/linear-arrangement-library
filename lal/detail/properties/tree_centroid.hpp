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
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/graphs/size_subtrees.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/pairs_utils.hpp>

namespace lal {
namespace detail {

namespace __lal {

/**
 * @brief Retrieves the centroid of the tree
 *
 * While looking for the centroid, this function updates @e L and @e sizes_edge.
 *
 * This function uses @ref lal::detail::calculate_bidirectional_sizes, an
 * algorithm described in \cite Hochberg2003a (see function's documentation for
 * details).
 *
 * @tparam tree_t The type of tree.
 * @param t Input tree.
 * @param N Actual number of vertices of the tree
 * @param n Number of vertices of the connected component of node @e x.
 * @param x Starting node. Node of the component whose centroid we want.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param[out] sizes_edge Auxiliary memory to construct @e L. For every edge
 * \f$\{u,v\}\f$ contains two pairs \f$((u,v), s)\f$ and \f$((v,u), n-s)\f$,
 * where \f$n\f$ is the number of vertices of the connected component of 'x'
 * and \f$s=|V(T^u_v)|\f$.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 * @post @e L is updated. It is sorted decreasingly.
 * @post @e sizes_edge is updated.
 */
template<
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
std::pair<node, node> retrieve_centroid(
	const tree_t& t,
	const uint64_t N, const uint64_t n, const node x,
	std::vector<std::vector<node_size>>& L,
	data_array<edge_size>& sizes_edge
)
noexcept
{
#if defined DEBUG
	assert(n > 0);
#endif

	typedef edge_size* iterator_t;

	{
	sizes_edge.resize(2*(n - 1));
	auto it = sizes_edge.begin();

	// calculate s(u,v) with H&S algorithm (lemma 8)
	detail::calculate_bidirectional_sizes
		<tree_t, iterator_t>
		(t, n, x, it);

	// sort all tuples in sizes_edge using the sizes s(u,v)
	detail::counting_sort
		<edge_size, iterator_t, countingsort::non_increasing_t>
		(
			sizes_edge.begin(), sizes_edge.end(), n, sizes_edge.size(),
			[](const edge_size& edge_pair) -> std::size_t
			{ return edge_pair.size; }
		);
		}

	// put the s(u,v) into an adjacency list
	// M[u] : adjacency list of vertex u sorted decreasingly according
	// to the sizes of the subtrees.
	L.resize(N);
	for (const auto& [uv, suv] : sizes_edge) {
		const auto [u, v] = uv;
		L[u].push_back({v, suv});
	}

	// find the first centroidal vertex
	node c1 = N + 1;
	bool found = false;
	node u = x;
	while (not found) {
		const auto& [v, suv] = L[u][0];
		u = (suv > n/2 ? v : u);
		found = suv <= n/2;
	}
	c1 = u;

#if defined DEBUG
	assert(c1 < N);
#endif

	// find the second centroidal vertex among the
	// neighbours of the first centroidal vertex
	node c2 = N + 1;
	for (const auto& p : L[c1]) {
		const node v = p.v;
		if (L[v][0].size <= n/2) {
			c2 = v;
			break;
		}
	}

	return (c1 < c2 ? std::make_pair(c1, c2) : std::make_pair(c2, c1));
}

} // -- namespace __lal

// -----------------------------------------------------------------------------

/**
 * @brief Calculate the centroid of the connected component that has node @e x.
 *
 * Here, "centroid" should NOT be confused with "centre". The centre is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. In both case, if
 * the set has two vertices then they are adjacent in the tree. See \cite Harary1969a
 * for further details.
 *
 * A graph of type @ref lal::graphs::tree may lack some edges tree so it can have
 * several connected components. Vertex @e x belongs to one of these connected
 * components. So, this method finds the centroidal nodes of the connected
 * component node @e x belongs to.
 *
 * This function calls @ref lal::detail::__lal::retrieve_centroid.
 *
 * @tparam tree_t Type of tree.
 * @param t Input tree.
 * @param x Input node.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param[out] sizes_edge Auxiliary memory to construct @e L. For every edge
 * \f$\{u,v\}\f$ contains two pairs \f$((u,v), s)\f$ and \f$((v,u), n-s)\f$,
 * where \f$n\f$ is the number of vertices of the connected component of @e x
 * and \f$s=|V(T^u_v)|\f$.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 * @post @e L is updated. It is sorted decreasingly.
 * @post @e sizes_edge is updated.
 */
template<
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool> = true
>
std::pair<node, node> retrieve_centroid(
	const tree_t& t, const node x,
	std::vector<std::vector<node_size>>& L,
	data_array<edge_size>& sizes_edge
)
noexcept
{
	// actual number of vertices of the tree
	const uint64_t component_size = t.get_num_nodes();
	// calculate the size of the connected component
	const uint64_t n = t.get_num_nodes_component(x);
	// easy case
	if (n == 1) { return {x, component_size + 1}; }
	// general case
	return __lal::retrieve_centroid(t, component_size, n, x, L, sizes_edge);
}

/**
 * @brief Calculate the centroid of the connected component that has node @e x.
 *
 * For details on the parameters and return value see documentation of the
 * function above.
 * @tparam tree_t Type of tree.
 * @param t Input tree
 * @param x Node belonging to a connected component whose centroid we want.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
std::pair<node, node> retrieve_centroid(const tree_t& t, const node x)
noexcept
{
	std::vector<std::vector<node_size>> M;
	data_array<edge_size> sizes_edge;
	return retrieve_centroid(t, x, M, sizes_edge);
}

// -----------------------------------------------------------------------------

/**
 * @brief Calculate the centroid of the tree @e t.
 *
 * Here, "centroid" should NOT be confused with "centre". The centre is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. In both case, if
 * the set has two vertices then they are adjacent in the tree. See \cite Harary1969a
 * for further details.
 *
 * @tparam tree_t Type of the input tree.
 * @param t Input tree.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @param[out] sizes_edge Auxiliary memory to construct @e L. For every edge
 * \f$\{u,v\}\f$ contains two pairs \f$((u,v), s)\f$ and \f$((v,u), n-s)\f$,
 * where \f$n\f$ is the number of vertices of the connected component of @e x
 * and \f$s=|V(T^u_v)|\f$.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 * @pre The tree @e t is a valid tree (see @ref lal::graphs::tree::is_tree).
 */
template<
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
std::pair<node, node> retrieve_centroid(
	const tree_t& t,
	std::vector<std::vector<node_size>>& L,
	data_array<edge_size>& sizes_edge
)
noexcept
{
	// actual number of vertices of the tree
	const uint64_t n = t.get_num_nodes();
	// easy case
	if (n == 1) { return {0, n+1}; }
	// general case
	return __lal::retrieve_centroid(t, n, n, 0, L, sizes_edge);
}

/**
 * @brief Calculate the centroid of the tree @e t.
 *
 * Here, "centroid" should NOT be confused with "centre". The centre is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. In both case, if
 * the set has two vertices then they are adjacent in the tree. See \cite Harary1969a
 * for further details.
 *
 * @tparam tree_t Type of the input tree.
 * @param t Input tree.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
std::pair<node, node> retrieve_centroid(const tree_t& t)
noexcept
{
	std::vector<std::vector<node_size>> L;
	data_array<edge_size> sizes_edge;
	return retrieve_centroid(t, L, sizes_edge);
}

/**
 * @brief Calculate the centroid of the tree @e t.
 *
 * Here, "centroid" should NOT be confused with "centre". The centre is the set
 * of (at most) two vertices that have minimum eccentricity. The centroid is the
 * set of (at most) two vertices that have minimum weight, where the weight is
 * the maximum size of the subtrees rooted at that vertex. In both case, if
 * the set has two vertices then they are adjacent in the tree. See \cite Harary1969a
 * for further details.
 *
 * @tparam tree_t Type of the input tree.
 * @param t Input tree.
 * @param[out] L Adjacency list-like data structure. \f$L[u]\f$ is a list of
 * pairs \f$(v, n_u(v))\f$ where \f$v\f$ is a neighbour of \f$u\f$ and
 * \f$n_u(v)=|V(T^u_v)|\f$ is the size of the subtree \f$T^u_v\f$ in vertices.
 * @returns A tuple of two values: the nodes in the centroid. If the
 * tree has a single centroidal node, only the first node is valid and the second
 * is assigned an invalid vertex index. It is guaranteed that the first vertex
 * has smaller index value than the second.
 */
template<
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
std::pair<node, node> retrieve_centroid
(const tree_t& t, std::vector<std::vector<node_size>>& L)
noexcept
{
	data_array<edge_size> sizes_edge;
	return retrieve_centroid(t, L, sizes_edge);
}

} // -- namespace detail
} // -- namespace lal
