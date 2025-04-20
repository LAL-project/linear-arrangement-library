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

//#define USE_COMPLICATED

// C++ includes
#if defined USE_COMPLICATED
#include <string>
#include <vector>
#endif

// lal includes
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/detail/properties/tree_centre.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/utilities/tree_isomorphism.hpp>
#include <lal/detail/macros/basic_convert.hpp>

namespace lal {
namespace utilities {

#if defined USE_COMPLICATED
template <typename T>
void free_memory(T *& m)
{
	if (m != nullptr) {
		delete[] m;
		m = nullptr;
	}
}

class test_isomorphism {
private:

	std::string *_pieces_of_node = nullptr;
	std::string *_pieces_of_level = nullptr;
	uint64_t *_distances = nullptr;

private:

	const rooted_tree &t1, t2;
	const uint64_t n;

	std::vector<std::vector<node>> nodes_per_level_t1;
	std::string *name_per_level_t1 = nullptr;
	std::string *name_per_node_t1 = nullptr;

	std::vector<std::vector<node>> nodes_per_level_t2;
	std::string *name_per_level_t2 = nullptr;
	std::string *name_per_node_t2 = nullptr;

public:

	test_isomorphism(const rooted_tree& _t1, const rooted_tree& _t2)
		: t1(_t1),
		  t2(_t2),
		  n(_t1.get_num_nodes())
	{
		_pieces_of_node = new std::string[n];
		_pieces_of_level = new std::string[n];
		_distances = new uint64_t[n];
	}

	~test_isomorphism()
	{
		free_memory(name_per_level_t1);
		free_memory(name_per_node_t1);
		free_memory(name_per_level_t2);
		free_memory(name_per_node_t2);
		free_memory(_pieces_of_node);
		free_memory(_pieces_of_level);
		free_memory(_distances);
	}

	bool are_trees_isomorphic()
	{
		make_names_per_level<false>(
			t1, nodes_per_level_t1, name_per_level_t1, name_per_node_t1
		);

		return make_names_per_level<true>(
			t2, nodes_per_level_t2, name_per_level_t2, name_per_node_t2
		);
	}

	void retrieve_nodes_per_level(
		const rooted_tree& t, std::vector<std::vector<node>>& nodes_per_level
	)
	{
		nodes_per_level = std::vector<std::vector<node>>(n);

		nodes_per_level[0].push_back(t.get_root());

		detail::BFS<rooted_tree> bfs(t);
		bfs.set_use_rev_edges(false);

		uint64_t max_level = 0;
		std::fill(&_distances[0], &_distances[n], 0);

		bfs.set_process_neighbour(
			[&](const auto&, node u, node v, bool) -> void
			{
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

	template <bool compare>
	using Type = typename std::conditional<compare, bool, void>::type;
	template <bool compare>
	Type<compare> make_names_per_level(
		const rooted_tree& t,
		std::vector<std::vector<node>>& nodes_per_level_t,
		std::string *& name_per_level_t,
		std::string *& name_per_node_t
	)
	{
		retrieve_nodes_per_level(t, nodes_per_level_t);
		if constexpr (compare) {
			if (nodes_per_level_t.size() != nodes_per_level_t1.size()) {
				return false;
			}
		}

		name_per_level_t = new std::string[nodes_per_level_t.size()];
		name_per_node_t = new std::string[n];

		for (int l = to_int(nodes_per_level_t.size()) - 1; l >= 0; --l) {
			const auto& nodes_level_l = nodes_per_level_t[l];
			const std::size_t n_nodes_in_level = nodes_level_l.size();

			std::size_t i = 0;
			for (node u : nodes_level_l) {
				// store the neighbors names in a vector for later sorting

				std::size_t j = 0;
				for (node v : t.get_out_neighbors(u)) {
					_pieces_of_node[j++] = name_per_node_t[v];
				}
				sort(&_pieces_of_node[0], &_pieces_of_node[j]);

				// build the name for this node
				name_per_node_t[u] = "1";
				for (std::size_t k = 0; k < j; ++k) {
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
			for (std::size_t k = 0; k < n_nodes_in_level; ++k) {
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

bool are_trees_isomorphic(const rooted_tree& t1, const rooted_tree& t2)
{
	const int discard = fast_non_iso(t1, t2);
	if (discard == 0) {
		return true;
	}
	if (discard == 1) {
		return false;
	}

	test_isomorphism ti(t1, t2);
	return ti.are_trees_isomorphic();
}
#endif

bool are_trees_isomorphic(
	const graphs::rooted_tree& t1, const graphs::rooted_tree& t2
) noexcept
{
#if defined DEBUG
	assert(t1.has_root());
	assert(t2.has_root());
#endif
	return detail::are_rooted_trees_isomorphic(t1, t2);
}

bool are_trees_isomorphic(
	const graphs::free_tree& t1, const graphs::free_tree& t2
) noexcept
{
#if defined LAL_REGISTER_BIBLIOGRAPHY
	bibliography::register_entry(bibliography::entries::Aho1974a);
#endif

	const auto discard = detail::fast_non_iso(t1, t2);
	if (discard == 0) {
		return true;
	}
	if (discard == 1) {
		return false;
	}

	const uint64_t n = t1.get_num_nodes();
	if (n == 3) {
		return true;
	}

	// find centres of the trees
	const auto c1 = detail::retrieve_centre(t1, 0);
	const auto c2 = detail::retrieve_centre(t2, 0);

	// check centre sizes
	const uint64_t size1 = (c1.second < n ? 2 : 1);
	const uint64_t size2 = (c2.second < n ? 2 : 1);
	if (size1 != size2) {
		return false;
	}

	const graphs::rooted_tree rt1(t1, c1.first);

	// the centres have only one vertex
	if (size1 == 1) {
		return are_trees_isomorphic(rt1, graphs::rooted_tree(t2, c2.first));
	}

	// the centres have two vertices

	// try with the first centre of the second tree
	const bool iso1 =
		are_trees_isomorphic(rt1, graphs::rooted_tree(t2, c2.first));
	if (iso1) {
		return true;
	}

	// try with the second centre of the second tree
	return are_trees_isomorphic(rt1, graphs::rooted_tree(t2, c2.second));
}

} // namespace utilities
} // namespace lal
