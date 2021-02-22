#pragma once

// C++ includes
#include <algorithm>
#include <cinttypes>
#if defined DEBUG
#include <cassert>
#endif
#include <iostream>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace internal {

/*
 * Returns whether the input trees are, might be, or are not isomorphic.
 *
 * Returns 0 if the trees ARE isomorphic
 * Returns 1 if the trees ARE NOT isomorphic:
 * - number of vertices do not coincide
 * - number of leaves do not coincide
 * - second moment of degree do not coincide
 * Returns 2 if the trees MIGHT BE isomorphic
 */
template<
	class T,
	std::enable_if_t<
		std::is_base_of_v<graphs::free_tree, T> ||
		std::is_base_of_v<graphs::rooted_tree, T>,
	bool> = true
>
inline
char fast_non_iso(const T& t1, const T& t2) noexcept {
	// check number of nodes
	if (t1.n_nodes() != t2.n_nodes()) { return 1; }

	if constexpr (std::is_base_of_v<T, lal::graphs::rooted_tree>) {
	// rooted trees must have correct orientation of edges
	if (not t1.is_orientation_valid() or not t2.is_orientation_valid()) {
		return false;
	}
	}

	const uint32_t n = t1.n_nodes();

	// trees ARE isomorphic
	if (n <= 2) { return 0; }

	uint32_t nL_t1 = 0; // number of leaves of t1
	uint32_t nL_t2 = 0; // number of leaves of t2
	uint64_t k2_t1 = 0; // sum of squared degrees of t1
	uint64_t k2_t2 = 0; // sum of squared degrees of t2
	uint64_t maxdeg_t1 = 0; // max degree of t1
	uint64_t maxdeg_t2 = 0; // max degree of t2
	for (node u = 0; u < n; ++u) {
		const uint64_t ku1 = static_cast<uint64_t>(t1.degree(u));
		const uint64_t ku2 = static_cast<uint64_t>(t2.degree(u));

		nL_t1 += t1.degree(u) == 1;
		nL_t2 += t2.degree(u) == 1;
		k2_t1 += ku1*ku1;
		k2_t2 += ku2*ku2;
		maxdeg_t1 = (maxdeg_t1 < ku1 ? ku1 : maxdeg_t1);
		maxdeg_t2 = (maxdeg_t2 < ku2 ? ku2 : maxdeg_t2);
	}

	// check number of leaves
	if (nL_t1 != nL_t2) { return 1; }
	// check maximum degree
	if (maxdeg_t1 != maxdeg_t2) { return 1; }
	// check sum of squared degrees
	if (k2_t1 != k2_t2) { return 1; }

	// trees MIGHT BE isomorphic
	return 2;
}

/*
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * This function stores the names of every node in the subtree rooted at 'u'.
 * This is useful if we want to make lots of comparisons between subtrees
 *
 * For further details on the algorithm, see \cite Aho1974a for further details.
 * @param t Input rooted tree
 * @param u Root of the subtree whose name we want to calculate
 * @param names An array of strings where the names are stored (as in a dynamic
 * programming algorithm). The size of this array must be at least the number of
 * vertices in the subtree of 't' rooted at 'u'. Actually, less memory suffices,
 * but I don't know how much less: better be safe than sorry.
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @names[idx+1] will contain the
 * name of the second child of 'u'.
 * @return Returns the code for the subtree rooted at 'u'.
 */
inline
void assign_name_and_keep(
	const graphs::rooted_tree& t, node u,
	std::string *aux_memory_for_names, size_t idx,
	std::string *keep_name_of
)
noexcept
{
	if (t.out_degree(u) == 0) {
		keep_name_of[u] = "10";
		return;
	}

	// make childrens' names
	const size_t begin_idx = idx;
	for (node v : t.get_out_neighbours(u)) {
		// make the name for v
		assign_name_and_keep(t,v, aux_memory_for_names, idx+1, keep_name_of);

		aux_memory_for_names[idx] = keep_name_of[v];
		++idx;
	}
	std::sort(&aux_memory_for_names[begin_idx], &aux_memory_for_names[idx]);

	// join the names in a single string to make the name of vertex 'v'
	std::string name = "1";
	for (size_t j = begin_idx; j < idx; ++j) {
		name += aux_memory_for_names[j];
	}
	name += "0";
	keep_name_of[u] = name;
}

/*
 * @brief Assigns a name to node 'u', root of the current subtree.
 *
 * For further details on the algorithm, see \cite Aho1974a for further details.
 * @param t Input rooted tree
 * @param u Root of the subtree whose name we want to calculate
 * @param names An array of strings where the names are stored (as in a dynamic
 * programming algorithm). The size of this array must be at least the number of
 * vertices in the subtree of 't' rooted at 'u'. Actually, less memory suffices,
 * but I don't know how much less: better be safe than sorry.
 * @param idx A pointer to the position within @e names that will contain the
 * name of the first child of 'u'. The position @names[idx+1] will contain the
 * name of the second child of 'u'.
 * @return Returns the code for the subtree rooted at 'u'.
 */
inline
std::string assign_name
(const graphs::rooted_tree& t, node u, std::string *names, size_t idx)
noexcept
{
	if (t.out_degree(u) == 0) {
		return std::string("10");
	}

	// make childrens' names
	const size_t begin_idx = idx;
	for (node v : t.get_out_neighbours(u)) {
		// make the name for v
		names[idx] = assign_name(t,v, names, idx+1);
		++idx;
	}
	std::sort(&names[begin_idx], &names[idx]);

	// join the names in a single string to make the name of vertex 'v'
	std::string name = "1";
	for (size_t j = begin_idx; j < idx; ++j) {
		name += names[j];
	}
	name += "0";

	return name;
}

inline
bool are_full_trees_isomorphic
(const graphs::rooted_tree& t1, const graphs::rooted_tree& t2)
noexcept
{
	const auto discard = internal::fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	const uint32_t n = t1.n_nodes();
	data_array<std::string> names(n);
	const std::string name_r1 = assign_name(t1, t1.get_root(), names.data, 0);
	const std::string name_r2 = assign_name(t2, t2.get_root(), names.data, 0);
	return name_r1 == name_r2;
}

} // -- namespace internal
} // -- namespace lal