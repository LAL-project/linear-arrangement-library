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

//#define USE_COMPLICATED
#undef USE_COMPLICATED

// C++ includes
#include <string>
#if defined USE_COMPLICATED
#include <vector>
#endif
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/tree_centre.hpp>
#include <lal/internal/data_array.hpp>
#include <lal/internal/graphs/trees/tree_isomorphism.hpp>

#define to_uint64(x) (static_cast<uint64_t>(x))
#define to_int(x) (static_cast<int>(x))

namespace lal {
using namespace graphs;

namespace utilities {

#if defined USE_COMPLICATED
template<typename T>
void free_memory(T*& m) {
	if (m != nullptr) { delete[] m; m = nullptr; }
}

class test_isomorphism {
private:
string *_pieces_of_node = nullptr;
string *_pieces_of_level = nullptr;
uint32_t *_distances = nullptr;

private:
const rooted_tree& t1, t2;
const uint32_t n;

vector<vector<node>> nodes_per_level_t1;
string *name_per_level_t1 = nullptr;
string *name_per_node_t1 = nullptr;

vector<vector<node>> nodes_per_level_t2;
string *name_per_level_t2 = nullptr;
string *name_per_node_t2 = nullptr;

public:
test_isomorphism(const rooted_tree& _t1, const rooted_tree& _t2)
: t1(_t1), t2(_t2), n(_t1.num_nodes())
{
	_pieces_of_node = new string[n];
	_pieces_of_level = new string[n];
	_distances = new uint32_t[n];
}

~test_isomorphism() {
	free_memory(name_per_level_t1);
	free_memory(name_per_node_t1);
	free_memory(name_per_level_t2);
	free_memory(name_per_node_t2);
	free_memory(_pieces_of_node);
	free_memory(_pieces_of_level);
	free_memory(_distances);
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
	nodes_per_level = vector<vector<node>>(n);

	nodes_per_level[0].push_back(t.get_root());

	internal::BFS<rooted_tree> bfs(t);
	bfs.set_use_rev_edges(false);

	uint32_t max_level = 0;
	std::fill(&_distances[0], &_distances[n], 0);

	bfs.set_process_neighbour(
	[&](const auto&, node u, node v, bool) -> void {
		_distances[v] = _distances[u] + 1;
		nodes_per_level[_distances[v]].push_back(v);
		max_level = std::max(max_level, _distances[v]);
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
	retrieve_nodes_per_level(t, nodes_per_level_t);
	if constexpr (compare) {
	if (nodes_per_level_t.size() != nodes_per_level_t1.size()) {
		return false;
	}
	}

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
				_pieces_of_node[j++] = name_per_node_t[v];
			}
			sort(&_pieces_of_node[0], &_pieces_of_node[j]);

			// build the name for this node
			name_per_node_t[u] = "1";
			for (size_t k = 0; k < j; ++k) {
				name_per_node_t[u] += _pieces_of_node[k];
			}
			name_per_node_t[u] += "0";

			// collect another piece of this level
			_pieces_of_level[i] = name_per_node_t[u];
			++i;
		}

		// sort the level's pieces
		sort(&_pieces_of_level[0], &_pieces_of_level[n_nodes_in_level]);

		// build the level's name
		for (size_t k = 0; k < n_nodes_in_level; ++k) {
			name_per_level_t[l] += _pieces_of_level[k];
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
};

bool are_trees_isomorphic(const rooted_tree& t1, const rooted_tree& t2) {
	const int discard = fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	test_isomorphism ti(t1, t2);
	return ti.are_trees_isomorphic();
}
#endif

bool are_trees_isomorphic(const rooted_tree& t1, const rooted_tree& t2) noexcept {
#if defined DEBUG
	assert(t1.has_root());
	assert(t2.has_root());
#endif
	return internal::are_full_trees_isomorphic(t1, t2);
}

bool are_trees_isomorphic(const free_tree& t1, const free_tree& t2) noexcept {
	const auto discard = internal::fast_non_iso(t1,t2);
	if (discard == 0) { return true; }
	if (discard == 1) { return false; }

	const uint32_t n = t1.num_nodes();
	if (n == 3) { return true; }

	// find centres of the trees
	const auto c1 = internal::retrieve_centre(t1, 0);
	const auto c2 = internal::retrieve_centre(t2, 0);

	// check centre sizes
	const uint32_t size1 = (c1.second < n ? 2 : 1);
	const uint32_t size2 = (c2.second < n ? 2 : 1);
	if (size1 != size2) { return false; }

	const rooted_tree rt1(t1, c1.first);

	// the centres have only one vertex
	if (size1 == 1) {
		return are_trees_isomorphic(rt1, rooted_tree(t2, c2.first));
	}

	// the centres have two vertices

	// try with the first centre of the second tree
	const bool iso1 = are_trees_isomorphic(rt1, rooted_tree(t2, c2.first));
	if (iso1) { return true; }

	// try with the second centre of the second tree
	return are_trees_isomorphic(rt1, rooted_tree(t2, c2.second));
}

} // -- namespace utilities
} // -- namespace lal
