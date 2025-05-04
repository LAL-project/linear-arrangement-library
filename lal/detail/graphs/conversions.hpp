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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <sstream>

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/basic_types.hpp>
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/arrangement_wrapper.hpp>

namespace lal {
namespace detail {

// -----------------------------------------------------------------------------
// -- EDGE LIST --

template <
	graphs::Tree tree_t,
	bool ensure_root_is_returned,
	bool free_tree_plus_root = ensure_root_is_returned and
							   std::is_same_v<tree_t, graphs::free_tree>>
[[nodiscard]] std::
	conditional_t<free_tree_plus_root, std::pair<tree_t, node>, tree_t>
	from_edge_list_to_tree(std::stringstream& ss) noexcept
{
	// Read an edge list: {1 2} {2 3} {4 5} ...
	// The edge list is assumed to be complete due to
	// the requirements of this function.

	uint64_t max_node_idx = 0;

	// parse the edge list to find the
	// number of vertices of the tree
	char curly_bracket;
	node u, v;
	while (ss >> curly_bracket) {
		ss >> u >> v >> curly_bracket;
		max_node_idx = std::max(max_node_idx, u);
		max_node_idx = std::max(max_node_idx, v);
	}
	ss.clear();
	ss.seekg(0);

	const auto num_nodes = max_node_idx + 1;

	// read the edge list (again...) to construct the tree
	// without using extra memory

	tree_t t(num_nodes);
	while (ss >> curly_bracket) {
		ss >> u >> v >> curly_bracket;
		t.add_edge_bulk(u, v);
	}
	t.finish_bulk_add_complete();

	if constexpr (std::is_same_v<tree_t, graphs::rooted_tree>) {
#if defined DEBUG
		/*
		 * If in the future the call to "finish_bulk_add_complete" above
		 * finds the root of the tree then this assertion should
		 * fail and thus giving us a heads up that the code that
		 * follows is completely useless, hence saving us some
		 * execution time...
		 */
		assert(not t.has_root());
#endif

		// find and set the root in linear time :( ...
		for (node w = 0; w < num_nodes; ++w) {
			if (t.get_in_degree(w) == 0) {
				t.set_root(w);
				break;
			}
		}

#if defined DEBUG
		assert(t.is_rooted_tree());
#endif
	}
	else {
#if defined DEBUG
		assert(t.is_tree());
#endif
	}

	if constexpr (free_tree_plus_root) {
		static_assert(std::is_same_v<tree_t, graphs::free_tree>);
		return {std::move(t), num_nodes + 1};
	}
	else {
		return t;
	}
}

/**
 * @brief Converts an edge list into a graph.
 *
 * An edge list is a list of pairs of indices, each index in the pair being
 * different and in \f$[0,n-1]\f$., where \f$n\f$ is the number of vertices
 * of the tree.
 *
 * Methods @ref lal::io::read_edge_list read an edge list from a file in disk.
 * @param edge_list An edge list.
 * @param normalize Should the graph be normalized?
 * @param check In case the graph is not to be normalized, should we check whether
 * it is nor not?
 * @returns Returns a lal::graphs::rooted_tree obtained from the head vector.
 * @pre No edge in the list is repeated.
 */
template <graphs::Graph graph_t>
[[nodiscard]] graph_t from_edge_list_to_graph(
	const edge_list& edge_list, const bool normalize, const bool check
) noexcept
{
	uint64_t max_vertex_index = 0;
	for (const edge& e : edge_list) {
		max_vertex_index = std::max(max_vertex_index, e.first);
		max_vertex_index = std::max(max_vertex_index, e.second);
	}
	const uint64_t num_nodes = 1 + max_vertex_index;
	graph_t g(num_nodes);
	g.set_edges(edge_list, normalize, check);
	return g;
}

// -----------------------------------------------------------------------------
// -- HEAD VECTOR --

/**
 * @brief Transforms a head vector in a directed graph.
 * @tparam graph_t The type of graph.
 * @param hv The input head vector.
 * @param normalize Normalize the grpah or not?
 * @param check Check whether the graph is normalized or not?
 * @returns A graph.
 */
template <graphs::Graph graph_t>
[[nodiscard]] graph_t from_head_vector_to_graph(
	const head_vector& hv, const bool normalize, const bool check
) noexcept
{
	const uint64_t n = hv.size();
	graph_t g(n);
	for (uint64_t i = 0; i < n; ++i) {
		if (hv[i] != 0) {
			// add the edge:
			// * i ranges in [0,n-1]
			// * hv[i] ranges in [1,n]
			g.add_edge_bulk(hv[i] - 1, i);
		}
	}
	if constexpr (std::is_base_of_v<graphs::tree, graph_t>) {
		g.finish_bulk_add_complete(normalize, check);
	}
	else {
		g.finish_bulk_add(normalize, check);
	}
	return g;
}

/**
 * @brief Transforms a head vector in a tree.
 *
 * Reads the head vector from a stream object.
 * @tparam tree_t The type of tree.
 * @param ss Stream to read the head vector from.
 * @returns Either a pair of free tree and its root, or a rooted tree.
 */
template <
	graphs::Tree tree_t,
	bool ensure_root_is_returned,
	bool free_tree_plus_root = ensure_root_is_returned and
							   std::is_same_v<tree_t, graphs::free_tree>>
[[nodiscard]] std::
	conditional_t<free_tree_plus_root, std::pair<tree_t, node>, tree_t>
	from_head_vector_to_tree(std::stringstream& ss) noexcept
{
	uint64_t num_nodes = 0;

	// parse the edge list to find the number of vertices of the tree
	node u;
	while (ss >> u) {
		++num_nodes;
	}
	ss.clear();
	ss.seekg(0);

	// construct the tree
	tree_t t(num_nodes);

	// root node of the tree
	node r = num_nodes + 1;

#if defined DEBUG
	uint64_t num_edges_added = 0;
#endif

	uint64_t i = 0;
	while (ss >> u) {
		if (u == 0) {
			// root, do nothing
			r = i;
		}
		else {
			// note:
			// * i ranges in [0,n-1]
			// * L[i] ranges in [1,n] (hence the '-1')

			// In the head vector the edge (i, hv[i] - 1) is an edge of an
			// anti-arborescence. Since for our rooted trees we need the edge
			// of an arborescence, then we add the edge as (hv[i] - 1, i).
			// For free trees the order of vertices does not matter.
			t.add_edge_bulk(u - 1, i);

#if defined DEBUG
			++num_edges_added;
#endif
		}
		++i;
	}

#if defined DEBUG
	// root must have been set
	assert(r < num_nodes);
	// amount of edges added must be 'n-1'
	assert(num_edges_added == num_nodes - 1);
#endif

	t.finish_bulk_add_complete();
	if constexpr (std::is_same_v<tree_t, graphs::rooted_tree>) {
		t.set_root(r);
#if defined DEBUG
		assert(t.is_rooted_tree());
#endif
	}
	else {
#if defined DEBUG
		assert(t.is_tree());
#endif
	}

	if constexpr (free_tree_plus_root) {
		static_assert(std::is_same_v<tree_t, graphs::free_tree>);
		return {std::move(t), r};
	}
	else {
		return t;
	}
}

/**
 * @brief Converts a head vector into a tree
 * @tparam tree_t Type of input tree
 * @param hv Input head vector.
 * @param normalize Normalize the resulting tree.
 * @param check Check whether the constructed tree is normalized.
 * @returns A @ref lal::graphs::rooted_tree or a pair of @ref lal::graphs::free_tree
 * and the root encoded in the head vector.
 */
template <
	graphs::Tree tree_t,
	bool is_rooted = std::is_base_of_v<graphs::rooted_tree, tree_t>>
[[nodiscard]] std::conditional_t<
	is_rooted,
	graphs::rooted_tree,
	std::pair<graphs::free_tree, node>>
from_head_vector_to_tree(
	const head_vector& hv, const bool normalize, const bool check
) noexcept
{
	if (hv.size() == 0) {
		if constexpr (is_rooted) {
			return graphs::rooted_tree(0);
		}
		else {
			return {graphs::free_tree(0), 0};
		}
	}
	if (hv.size() == 1) {
#if defined DEBUG
		// the only vertex can only be the root
		assert(hv[0] == 0);
#endif
		if constexpr (is_rooted) {
			return graphs::rooted_tree(1);
		}
		else {
			return {graphs::free_tree(1), 0};
		}
	}

	const uint64_t num_nodes = hv.size();

	// output tree
	tree_t t(num_nodes);

	// root node of the tree
	node r = num_nodes + 1;

#if defined DEBUG
	uint64_t num_edges_added = 0;
#endif

	for (uint64_t i = 0; i < num_nodes; ++i) {
		if (hv[i] == 0) {
			// root, do nothing
			r = i;
		}
		else {
			// note:
			// * i ranges in [0,n-1]
			// * L[i] ranges in [1,n] (hence the '-1')

			// In the head vector the edge (i, hv[i] - 1) is an edge of an
			// anti-arborescence. Since for our rooted trees we need the edge
			// of an arborescence, then we add the edge as (hv[i] - 1, i).
			// For free trees the order of vertices does not matter.
			t.add_edge_bulk(hv[i] - 1, i);

#if defined DEBUG
			++num_edges_added;
#endif
		}
	}

#if defined DEBUG
	// root must have been set
	assert(r < num_nodes);
	// amount of edges added must be 'n-1'
	assert(num_edges_added == num_nodes - 1);
#endif

	t.finish_bulk_add_complete(normalize, check);

	if constexpr (is_rooted) {
		t.set_root(r);
#if defined DEBUG
		assert(t.is_rooted_tree());
#endif

		return t;
	}
	else {
#if defined DEBUG
		assert(t.is_tree());
#endif

		return {std::move(t), r};
	}
}

/**
 * @brief Constructs the head vector representation of a tree.
 * @tparam arrangement_t Type of arrangement.
 * @param t Input rooted tree.
 * @param arr Linear arrangement of the vertices.
 * @returns A head vector encoding the tree.
 */
template <Arrangement arrangement_t>
[[nodiscard]] head_vector from_tree_to_head_vector(
	const graphs::rooted_tree& t, const arrangement_t& arr
) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	head_vector hv(n);
	for (position_t p = 0ull; p < n; ++p) {
		const node u = arr[p];
		if (t.get_root() == u) {
			hv[*p] = 0;
		}
		else {
			const node_t parent = t.get_in_neighbors(u)[0];
			hv[*p] = arr[parent] + 1;
		}
	}
	return hv;
}

/**
 * @brief Constructs the head vector representation of a tree.
 * @tparam arrangement_t Type of arrangement.
 * @param t Input free tree.
 * @param r Root of the tree.
 * @param arr Linear arrangement of the vertices.
 * @returns A head vector
 */
template <detail::Arrangement arrangement_t>
[[nodiscard]] head_vector from_tree_to_head_vector(
	const graphs::free_tree& t, const arrangement_t& arr, const node r
) noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif

	return from_tree_to_head_vector(
		graphs::rooted_tree(t, r, false, false), arr
	);
}

// -----------------------------------------------------------------------------
// -- LEVEL SEQUENCE --

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * Examples of level sequences:
 *	- linear tree of n nodes:
	@verbatim
		0 1 2 3 4 ... (n-1) n
	@endverbatim
 *	- star tree of n nodes
	@verbatim
		0 1 2 2 2 .... 2 2
		   |------------| > (n-1) two's
	@endverbatim
 *
 * The concept of level sequence is defined in \cite Wright1986a.
 * @tparam tree_t The type of tree.
 * @param L The level sequence, in preorder.
 * @param n Number of nodes of the tree.
 * @param normalize Should the tree be normalized?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with the sequence level @e L.
 * @pre n >= 2.
 * @pre The size of L is exactly @e n + 1.
 * @pre The first value of a sequence must be a zero.
 * @pre The second value of a sequence must be a one.
 */
template <graphs::Tree tree_t>
[[nodiscard]] tree_t from_level_sequence_to_tree_small(
	const uint64_t * const L,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Wright1986a);
#endif

#if defined DEBUG
	// a little sanity check
	assert(L[0] == 0);
	assert(L[1] == 1);
#endif

	tree_t t(n);

	// 'stack' of root candidates: node at every level in {1,...,N}.
	// at position j, lev[j] contains the last node added at level j.
	array<node> root_candidates(n + 1, 0);
	std::size_t stack_it = 0;

	// evidently,
	root_candidates[0] = 1;
	for (node i = 2; i <= n; ++i) {
		// find in the stack the node which has to be connected to node 'i'.
		if (root_candidates[stack_it] + 2 > L[i]) {
			stack_it = L[i] - 2 + 1;
		}

		// the top of the stack is the parent of this node
		const node r = root_candidates[stack_it];

		// add edge...
		const auto [u, v] = (r == 0 ? edge(r, i - 1) : edge(r - 1, i - 1));
		t.add_edge_bulk(u, v);

		// the last node added at level L[i] is i.
		++stack_it;
#if defined DEBUG
		assert(stack_it == L[i]);
#endif
		root_candidates[stack_it] = i;
	}

	if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
		t.set_root(0);
	}

	t.finish_bulk_add_complete(normalize, check);

#if defined DEBUG
	assert(t.is_tree());
#endif

	return t;
}

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * Examples of level sequences:
 *	- linear tree of n nodes:
	@verbatim
		0 1 2 3 4 ... (n-1) n
	@endverbatim
 *	- star tree of n nodes
	@verbatim
		0 1 2 2 2 .... 2 2
		   |------------| > (n-1) two's
	@endverbatim
 *
 * The concept of level sequence is defined in \cite Wright1986a.
 * @tparam tree_t The type of tree.
 * @param L The level sequence, in preorder.
 * @param n Number of nodes of the tree.
 * @param normalize Should the tree be normalized?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with the sequence level @e L.
 * @pre n >= 2.
 * @pre The size of L is exactly @e n + 1.
 * @pre The first value of a sequence must be a zero.
 * @pre The second value of a sequence must be a one.
 */
template <graphs::Tree tree_t>
[[nodiscard]] tree_t from_level_sequence_to_tree_large(
	const uint64_t * const L,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Wright1986a);
#endif

#if defined DEBUG
	// a little sanity check
	assert(L[0] == 0);
	assert(L[1] == 1);
#endif

	// 'stack' of root candidates: node at every level in {1,...,N}.
	// at position j, lev[j] contains the last node added at level j.
	array<node> root_candidates(n + 1, 0);
	std::size_t stack_it = 0;

	array<edge> edge_list(n - 1);
	std::size_t edge_it = 0;
	array<uint64_t> vertex_degrees(n, 0);

	// evidently,
	root_candidates[0] = 1;
	for (node i = 2; i <= n; ++i) {
		// find in the stack the node which has to be connected to node 'i'.
		if (root_candidates[stack_it] + 2 > L[i]) {
			stack_it = L[i] - 2 + 1;
		}

		// the top of the stack is the parent of this node
		const node r = root_candidates[stack_it];

		// retrieve and store the edge, calculate vertex degrees
		const edge e = (r == 0 ? edge(r, i - 1) : edge(r - 1, i - 1));
		edge_list[edge_it++] = e;
		++vertex_degrees[e.first];
		++vertex_degrees[e.second];

		// the last node added at level L[i] is i.
		++stack_it;
#if defined DEBUG
		assert(stack_it == L[i]);
#endif
		root_candidates[stack_it] = i;
	}

#if defined DEBUG
	assert(edge_it == n - 1);
#endif

	tree_t t(n);
	for (node u = 0; u < n; ++u) {
		if constexpr (std::is_base_of_v<graphs::free_tree, tree_t>) {
			t.reserve_degree(u, vertex_degrees[u]);
		}
		else {
			t.reserve_out_degree(u, vertex_degrees[u] - (u != 0));
			t.reserve_in_degree(u, u != 0);
		}
	}
	for (const edge& e : edge_list) {
		t.add_edge_bulk(e.first, e.second);
	}

	if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
		t.set_root(0);
	}

#if defined DEBUG
	assert(t.is_tree());
	if constexpr (std::is_base_of_v<graphs::rooted_tree, tree_t>) {
		assert(t.has_root());
	}
#endif

	t.finish_bulk_add_complete(normalize, check);
	return t;
}

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * Examples of level sequences:
 *	- linear tree of n nodes:
	@verbatim
		0 1 2 3 4 ... (n-1) n
	@endverbatim
 *	- star tree of n nodes
	@verbatim
		0 1 2 2 2 .... 2 2
			|------------| > (n-1) two's
	@endverbatim
 *
 * The concept of level sequence is defined in \cite Wright1986a.
 * @tparam tree_t The type of tree.
 * @param L The level sequence, in preorder.
 * @param n Number of nodes of the tree.
 * @param normalize Should the tree be normalized?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with the sequence level @e L.
 * @pre n >= 2.
 * @pre The size of L is exactly @e n + 1.
 * @pre The first value of a sequence must be a zero.
 * @pre The second value of a sequence must be a one.
 */
template <graphs::Tree tree_t>
[[nodiscard]] tree_t from_level_sequence_to_tree(
	const uint64_t * const L,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
	return n <= 7 ? from_level_sequence_to_tree_small<tree_t>(
						L, n, normalize, check
					)
				  : from_level_sequence_to_tree_large<tree_t>(
						L, n, normalize, check
					);
}

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * See @ref lal::detail::from_level_sequence_to_tree for further details.
 */
template <graphs::Tree tree_t>
[[nodiscard]] tree_t from_level_sequence_to_tree_small(
	const std::vector<uint64_t>& L,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
	return from_level_sequence_to_tree_small<tree_t>(
		&L[0], n, normalize, check
	);
}

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * See @ref lal::detail::from_level_sequence_to_tree for further details.
 */
template <graphs::Tree tree_t>
[[nodiscard]] tree_t from_level_sequence_to_tree_large(
	const std::vector<uint64_t>& L,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
	return from_level_sequence_to_tree_large<tree_t>(
		&L[0], n, normalize, check
	);
}

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * See @ref lal::detail::from_level_sequence_to_tree for further details.
 */
template <graphs::Tree tree_t>
[[nodiscard]] tree_t from_level_sequence_to_tree(
	const std::vector<uint64_t>& L,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
	return n <= 7 ? from_level_sequence_to_tree_small<tree_t>(
						&L[0], n, normalize, check
					)
				  : from_level_sequence_to_tree_large<tree_t>(
						&L[0], n, normalize, check
					);
}

// -----------------------------------------------------------------------------
// -- PRUFER SEQUENCE --

/**
 * @brief Converts the Prüfer sequence of a labelled tree into a tree structure.
 *
 * For details on Prüfer sequences, see \cite Pruefer1918a.
 * @param seq The Prufer sequence sequence.
 * @param n Number of nodes of the tree.
 * @param normalize Should the tree be normalized?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with @e seq.
 */
[[nodiscard]] inline graphs::free_tree from_Prufer_sequence_to_ftree(
	const uint64_t * const seq,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Pruefer1918a);
#endif

	// initialisation
	const uint64_t L = n - 2;
	array<uint64_t> degree(n, 1);
	for (uint64_t i = 0; i < L; ++i) {
		degree[seq[i]] += 1;
	}

	// the output tree
	graphs::free_tree t(n);

	// TODO: can these nested loops be optimized with pointers?

	// for each number in the sequence seq[i], find the first
	// lowest-numbered node, j, with degree equal to 1, add
	// the edge (j, seq[i]) to the tree, and decrement the degrees
	// of j and seq[i].
	for (uint64_t v = 0; v < L; ++v) {
		const auto value = seq[v];
		bool node_found = false;
		node w = 0;
		while (w < n and not node_found) {
			if (degree[w] == 1) {
				t.add_edge_bulk(value, w);

				degree[value] -= 1;
				degree[w] -= 1;
				node_found = true;
			}
			++w;
		}
	}

	// two nodes u,v with degree 1 remain. Find them
	node u, v;
	u = v = n;
	for (node w = 0; w < n; ++w) {
		if (degree[w] == 1) {
			if (u == n) {
				u = w;
			}
			else {
				v = w;
			}
		}
	}

	// add edge (u,v) to the tree
	t.add_edge_bulk(u, v);
	t.finish_bulk_add_complete(normalize, check);
	return t;
}

/**
 * @brief Converts the Prüfer sequence of a labelled tree into a tree structure.
 *
 * For details on Prüfer sequences, see \cite Pruefer1918a.
 * @param seq The Prufer sequence sequence.
 * @param n Number of nodes of the tree.
 * @param normalize Should the tree be normalized?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with @e seq.
 */
[[nodiscard]] inline graphs::free_tree from_Prufer_sequence_to_ftree(
	const std::vector<uint64_t>& seq,
	const uint64_t n,
	const bool normalize,
	const bool check
) noexcept
{
	return from_Prufer_sequence_to_ftree(&seq[0], n, normalize, check);
}

} // namespace detail
} // namespace lal
