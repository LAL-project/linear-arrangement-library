/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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

#include <lal/utilities/tree_isomorphism.hpp>

// C++ includes
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/tree_centre.hpp>

#define to_uint64(x) (static_cast<uint64_t>(x))
#define to_int(x) (static_cast<int>(x))

//#define USE_COMPLICATED
#undef USE_COMPLICATED

namespace lal {
using namespace graphs;

namespace utilities {

/* Returns whether the input trees are, might be, or are not isomorphic.
 *
 * Returns 0 if the trees ARE isomorphic
 * Returns 1 if the trees ARE NOT isomorphic:
 * - number of vertices do not coincide
 * - number of leaves do not coincide
 * - second moment of degree do not coincide
 * Returns 2 if the trees MIGHT BE isomorphic
 */
template<class T>
int fast_non_iso(const T& t1, const T& t2) {
	// check number of nodes
	if (t1.n_nodes() != t2.n_nodes()) { return 1; }

	if constexpr (std::is_same_v<T, lal::graphs::rooted_tree>) {
	if (t1.is_orientation_valid() != t2.is_orientation_valid()) {
		return false;
	}
	}

	const uint32_t n = t1.n_nodes();
	if (n <= 2) { return 0; }

	uint32_t nL_t1 = 0; // number of leaves of t1
	uint32_t nL_t2 = 0; // number of leaves of t2
	uint64_t k2_t1 = 0; // sum of squared degrees of t1
	uint64_t k2_t2 = 0; // sum of squared degrees of t2
	uint64_t maxdeg_t1 = 0; // max degree of t1
	uint64_t maxdeg_t2 = 0; // max degree of t2
	for (node u = 0; u < n; ++u) {
		const uint64_t ku1 = to_uint64(t1.degree(u));
		const uint64_t ku2 = to_uint64(t2.degree(u));

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

	return 2;
}

// -----------------------------------------------------------------------------

#if defined USE_COMPLICATED
template<typename T>
void free_memory(T*& m) {
	if (m != nullptr) { delete[] m; m = nullptr; }
}

class test_isomorphism {
public:
	test_isomorphism(const rooted_tree& __t1, const rooted_tree& __t2)
	: t1(__t1), t2(__t2)
	{
		const uint32_t n = t1.n_nodes();
		__pieces_of_node = new string[n];
		__pieces_of_level = new string[n];
		__distances = new uint32_t[n];
	}

	~test_isomorphism() {
		free_memory(name_per_level_t1);
		free_memory(name_per_node_t1);
		free_memory(name_per_level_t2);
		free_memory(name_per_node_t2);
		free_memory(__pieces_of_node);
		free_memory(__pieces_of_level);
		free_memory(__distances);
	}

	bool are_trees_isomorphic() {
		make_names_per_level<false>
		(t1, nodes_per_level_t1, name_per_level_t1, name_per_node_t1);

		return
		make_names_per_level<true>
		(t2, nodes_per_level_t2, name_per_level_t2, name_per_node_t2);
	}

	void retrieve_nodes_per_level(
		const rooted_tree& t, vector<vector<node>>& nodes_per_level
	)
	{
		const uint32_t n = t.n_nodes();
		nodes_per_level = vector<vector<node>>(n);

		nodes_per_level[0].push_back(t.get_root());

		internal::BFS<rooted_tree> bfs(t);
		bfs.set_use_rev_edges(false);

		uint32_t max_level = 0;
		std::fill(&__distances[0], &__distances[n], 0);

		bfs.set_process_neighbour(
		[&](const auto&, node u, node v, bool) -> void {
			__distances[v] = __distances[u] + 1;
			nodes_per_level[__distances[v]].push_back(v);
			max_level = std::max(max_level, __distances[v]);
		}
		);
		bfs.start_at(t.get_root());

		nodes_per_level.resize(max_level + 1);
#if defined DEBUG
		assert(nodes_per_level.size() == max_level + 1);
#endif
	}

	template<bool compare>
	using Type = typename std::conditional<compare, bool, void>::type;
	template<bool compare> Type<compare>
	make_names_per_level
	(
		const rooted_tree& t,
		vector<vector<node>>& nodes_per_level_t,
		string*& name_per_level_t,
		string*& name_per_node_t
	)
	{
		const uint32_t n = t.n_nodes();

		retrieve_nodes_per_level(t, nodes_per_level_t);
		name_per_level_t = new string[nodes_per_level_t.size()];
		name_per_node_t = new string[n];

		for (int l = to_int(nodes_per_level_t.size()) - 1; l >= 0; --l) {
			const auto& nodes_level_l = nodes_per_level_t[l];
			const size_t n_nodes_in_level = nodes_level_l.size();

			size_t i = 0;
			for (node u : nodes_level_l) {
				// store the neighbours names in a vector for later sorting

				size_t j = 0;
				for (node v : t.get_out_neighbours(u)) {
					__pieces_of_node[j++] = name_per_node_t[v];
				}
				sort(&__pieces_of_node[0], &__pieces_of_node[j]);

				// build the name for this node
				name_per_node_t[u] = "1";
				for (size_t k = 0; k < j; ++k) {
					name_per_node_t[u] += __pieces_of_node[k];
				}
				name_per_node_t[u] += "0";

				// collect another piece of this level
				__pieces_of_level[i] = name_per_node_t[u];
				++i;
			}

			// sort the level's pieces
			sort(&__pieces_of_level[0], &__pieces_of_level[n_nodes_in_level]);

			// build the level's name
			for (size_t k = 0; k < n_nodes_in_level; ++k) {
				name_per_level_t[l] += __pieces_of_level[k];
			}

			if constexpr (compare) {
			if (name_per_level_t[l] != name_per_level_t1[l]) {
				return false;
			}
			}
		}


		if constexpr (compare) {
			return true;
		}
	}

private:
	string *__pieces_of_node = nullptr;
	string *__pieces_of_level = nullptr;
	uint32_t *__distances = nullptr;

private:
	const rooted_tree& t1, t2;

	vector<vector<node>> nodes_per_level_t1;
	string *name_per_level_t1 = nullptr;
	string *name_per_node_t1 = nullptr;

	vector<vector<node>> nodes_per_level_t2;
	string *name_per_level_t2 = nullptr;
	string *name_per_node_t2 = nullptr;
};

bool are_trees_isomorphic(const rooted_tree& t1, const rooted_tree& t2) {
	const int discard = fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	test_isomorphism ti(t1, t2);
	return ti.are_trees_isomorphic();
}
#else
void assign_name(const rooted_tree& t, node v, string& name) {
	if (t.degree(v) == 0) {
		name = "10";
		return;
	}

	// make childrens' names
	vector<string> names_children(t.degree(v));
	size_t i = 0;
	for (node u : t.get_neighbours(v)) {
		assign_name(t,u, names_children[i]);
		++i;
	}
	sort(names_children.begin(), names_children.end());

	// join the names in a single string
	name = "1";
	for (const string& nc : names_children) {
		name += nc;
	}
	name += "0";

	names_children.clear();
}

bool are_trees_isomorphic(const rooted_tree& t1, const rooted_tree& t2) {
	const int discard = fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	const node r1 = t1.get_root();
	const node r2 = t2.get_root();

	string name_r1, name_r2;
	assign_name(t1, r1, name_r1);
	assign_name(t2, r2, name_r2);
	return name_r1 == name_r2;
}
#endif

bool are_trees_isomorphic(const free_tree& t1, const free_tree& t2) {
	const int discard = fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	const uint32_t n = t1.n_nodes();
	if (n == 3) { return true; }

	// find centres of the trees
	const auto c1 = internal::retrieve_centre(t1, 0);
	const auto c2 = internal::retrieve_centre(t2, 0);

	// check centre sizes
	const uint32_t size1 = (c1.second < n ? 2 : 1);
	const uint32_t size2 = (c2.second < n ? 2 : 1);
	if (size1 != size2) { return false; }

	const rooted_tree rt1 = rooted_tree(t1, c1.first);

	// the centres have only one vertex
	if (size1 == 1) {
		const rooted_tree rt2 = rooted_tree(t2, c2.first);
		return are_trees_isomorphic(rt1, rt2);
	}

	// the centres have two vertices

	// try with the first centre of the second tree
	const rooted_tree rt2_1 = rooted_tree(t2, c2.first);
	const bool iso1 = are_trees_isomorphic(rt1, rt2_1);
	if (iso1) { return true; }

	// try with the second centre of the second tree
	const rooted_tree rt2_2 = rooted_tree(t2, c2.second);
	return are_trees_isomorphic(rt1, rt2_2);
}

} // -- namespace utilities
} // -- namespace lal
