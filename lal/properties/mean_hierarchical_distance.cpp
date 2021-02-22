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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/internal/graphs/traversal.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
using namespace graphs;
using namespace numeric;

namespace properties {

rational mean_hierarchical_distance_rational(const rooted_tree& tree) {
	const uint32_t n = tree.n_nodes();

	uint32_t sum_distances = 0;
	internal::data_array<uint32_t> distances(n, 0);

	internal::BFS<rooted_tree> bfs(tree);
	bfs.set_process_neighbour(
	[&](const auto&, const node s, const node t, bool) -> void {
		distances[t] = distances[s] + 1;
		sum_distances += distances[t];
	}
	);
	bfs.start_at(tree.get_root());

	return rational(sum_distances, tree.n_edges());
}

double mean_hierarchical_distance(const rooted_tree& t) {
	return mean_hierarchical_distance_rational(t).to_double();
}

} // -- namespace properties
} // -- namespace lal
