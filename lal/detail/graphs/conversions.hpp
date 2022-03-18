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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <optional>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/data_array.hpp>

namespace lal {
namespace detail {

// -----------------------------------------------------------------------------
// -- HEAD VECTOR --

/**
 * @brief Constructs the head vector representation of a tree.
 * @param t Input rooted tree.
 * @returns A head vector encoding the tree.
 */
inline
head_vector from_tree_to_head_vector(const graphs::rooted_tree& t) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif

	const uint64_t n = t.get_num_nodes();
	head_vector hv(n);
	for (node u = 0; u < n; ++u) {
		if (u == t.get_root()) {
			hv[u] = 0;
		}
		else {
			// this is guaranteed to be a legal access (within bounds).
			hv[u] = t.get_in_neighbours(u)[0] + 1;
		}
	}
	return hv;
}

/**
 * @brief Constructs the head vector representation of a tree.
 * @param t Input free tree.
 * @param r Root of the tree.
 * @returns A head vector
 */
inline
head_vector from_tree_to_head_vector(const graphs::free_tree& t, node r) noexcept
{
#if defined DEBUG
	assert(t.is_tree());
#endif
	return from_tree_to_head_vector(graphs::rooted_tree(t,r));
}

/**
 * @brief Converts a head vector into a tree
 * @tparam tree_t Type of input tree
 * @param hv Input head vector.
 * @param normalise Normalise the resulting tree.
 * @param check Check whether the constructed tree is normalised.
 * @returns A @ref lal::graphs::rooted_tree or a pair of @ref lal::graphs::free_tree
 * and the root encoded in the head vector.
 */
template<
	class tree_t,
	bool is_rooted = std::is_base_of_v<graphs::rooted_tree, tree_t>
>
std::conditional_t<
	is_rooted,
	graphs::rooted_tree,
	std::pair<graphs::free_tree,node>
>
from_head_vector_to_tree(const head_vector& hv, bool normalise, bool check)
noexcept
{
	if (hv.size() == 0) {
		if constexpr (is_rooted) {
			return graphs::rooted_tree(0);
		}
		else {
			return std::make_pair(graphs::free_tree(0), 0);
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
			return std::make_pair(graphs::free_tree(1), 0);
		}
	}

	const uint64_t n = hv.size();

	// output tree
	tree_t t(n);

	// root node of the tree
	std::optional<node> r;

#if defined DEBUG
	uint64_t num_edges_added = 0;
#endif

	for (uint64_t i = 0; i < n; ++i) {
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
	assert(r);
	// amount of edges added must be 'n-1'
	assert(num_edges_added == n - 1);
#endif

	t.finish_bulk_add(normalise, check);

	if constexpr (is_rooted) {
		t.set_root(*r);
#if defined DEBUG
		assert(t.is_rooted_tree());
#endif

		return t;
	}
	else {
#if defined DEBUG
		assert(t.is_tree());
#endif

		return std::make_pair(t, *r);
	}
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
 * @param L The level sequence, in preorder.
 * @param n Number of nodes of the tree.
 * @param normalise Should the tree be normalised?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with the sequence level @e L.
 * @pre n >= 2.
 * @pre The size of L is exactly @e n + 1.
 * @pre The first value of a sequence must be a zero.
 * @pre The second value of a sequence must be a one.
 */
inline
graphs::free_tree level_sequence_to_ftree
(const uint64_t * const L, uint64_t n, bool normalise = true, bool check = true)
noexcept
{
#if defined DEBUG
	// a little sanity check
	assert(L[0] == 0);
	assert(L[1] == 1);
#endif

	// output tree
	graphs::free_tree t(n);

	// 'stack' of root candidates: node at every level in {1,...,N}.
	// at position j, lev[j] contains the last node added at level j.
	data_array<node> lev(n+1, 0);
	uint64_t stack_it = 0;

	// evidently,
	lev[0] = 1;

	for (node i = 2; i <= n; ++i) {
		// find in the stack the node which
		// has to be connected to node 'i'.
		if (lev[stack_it] + 2 > L[i]) {
			stack_it = L[i] - 2 + 1;
		}

		// the top of the stack is the parent of this node
		const node r = lev[stack_it];

		// add the edge...
		const auto [u,v] = (r == 0 ? edge(r, i - 1) : edge(r - 1, i - 1));
		t.add_edge_bulk(u, v);

		// the last node added at level L[i] is i.
		++stack_it;
#if defined DEBUG
		assert(stack_it == L[i]);
#endif
		lev[stack_it] = i;
	}

	t.finish_bulk_add(normalise, check);
	return t;
}

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * See @ref lal::detail::level_sequence_to_ftree(const uint64_t*, uint64_t, bool, bool)
 * for further details.
 */
inline
graphs::free_tree level_sequence_to_ftree
(const std::vector<uint64_t>& L, uint64_t n, bool normalise = true, bool check = true)
noexcept
{ return level_sequence_to_ftree(&L[0], n, normalise, check); }

// -----------------------------------------------------------------------------
// -- PRUFER SEQUENCE --

/**
 * @brief Converts the Prüfer sequence of a labelled tree into a tree structure.
 *
 * For details on Prüfer sequences, see \cite Pruefer1918a.
 * @param seq The Prufer sequence sequence.
 * @param n Number of nodes of the tree.
 * @param normalise Should the tree be normalised?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with @e seq.
 */
inline
graphs::free_tree Prufer_sequence_to_ftree
(const uint64_t * const seq, uint64_t n, bool normalise = true, bool check = true)
noexcept
{
	// initialisation
	const uint64_t L = n - 2;
	data_array<uint64_t> degree(n, 1);
	for (uint64_t i = 0; i < L; ++i) {
		degree[ seq[i] ] += 1;
	}

	// the output tree
	graphs::free_tree t(n);

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
	t.finish_bulk_add(normalise, check);
	return t;
}

/**
 * @brief Converts the Prüfer sequence of a labelled tree into a tree structure.
 *
 * For details on Prüfer sequences, see \cite Pruefer1918a.
 * @param seq The Prufer sequence sequence.
 * @param n Number of nodes of the tree.
 * @param normalise Should the tree be normalised?
 * @param check Should it be checked whether the tree is normalized or not?
 * @returns The tree built with @e seq.
 */
inline
graphs::free_tree Prufer_sequence_to_ftree
(const std::vector<uint64_t>& seq, uint64_t n, bool normalise = true, bool check = true)
noexcept
{ return Prufer_sequence_to_ftree(&seq[0], n, normalise, check); }


} // -- namespace detail
} // -- namespace lal
