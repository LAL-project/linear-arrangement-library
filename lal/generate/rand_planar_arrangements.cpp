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

#include <lal/generate/rand_planar_arrangements.hpp>

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <vector>
using namespace std;

// lal includes
#include <lal/internal/graphs/make_arrangement.hpp>

namespace lal {
using namespace graphs;
using namespace internal;

namespace generate {

rand_planar_arrangements::rand_planar_arrangements
(const free_tree& T, uint32_t seed) noexcept
	: m_T(T)
{
#if defined DEBUG
	assert(m_T.is_tree());
#endif
	if (seed == 0) {
		random_device rd;
		m_gen = mt19937(rd());
	}
	else {
		m_gen = mt19937(seed);
	}

	// initialise the random data of all vertices
	m_rdata = vector<vector<node>>(m_T.get_num_nodes());
}

template<class GEN>
void make_random_projective(
	const graphs::free_tree& T,
	node parent_u, node u,
	// Its size must be equal to the number of vertices of the tree.
	vector<vector<node>>& data,
	// random number generator
	GEN& gen
)
{
	// number of neighbours
	//  +1  : because we add 'u' to the interval
	//  -1  : because we remove the parent of u
	const uint32_t d_out = T.get_degree(u) + 1 - 1;
	const neighbourhood& neighs_u = T.get_neighbours(u);

	// Choose random positions for the intervals corresponding to the
	// vertex 'r' and to the trees rooted at 'r's children. These choices
	// have to be made with respect to 'r'. Remember: there are n_children+1
	// possibilities.

	data[u] = vector<node>(d_out);

	// fill interval with the root vertex and its children
	auto& inter = data[u];
	inter[0] = u;
	copy_if(
		neighs_u.begin(), neighs_u.end(), inter.begin() + 1,
		[&](node v) -> bool { return v != parent_u; }
	);

	// shuffle the positions
	shuffle(inter.begin(), inter.end(), gen);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them inductively.
	for (const node& v : neighs_u) {
		if (v != parent_u) {
			make_random_projective(T, u, v, data, gen);
		}
	}
}

linear_arrangement rand_planar_arrangements::get_arrangement() noexcept {
	uniform_int_distribution<node> U(0, m_T.get_num_nodes() - 1);
	const node root = U(m_gen);

	// number of children of 'r' with respect to the tree's root
	const uint32_t d_out = m_T.get_degree(root) + 1;
	const neighbourhood& neighs_root = m_T.get_neighbours(root);

	// Choose random positions for the intervals corresponding to the
	// vertex 'r' and to the trees rooted at 'r's children. These choices
	// have to be made with respect to 'r'. Remember: there are n_children+1
	// possibilities.

	m_rdata[root] = vector<node>(d_out);

	// fill interval with the root vertex and its children
	auto& inter = m_rdata[root];
	inter[0] = root;
	copy(neighs_root.begin(), neighs_root.end(), inter.begin() + 1);

	// shuffle the positions
	shuffle(inter.begin() + 1, inter.end(), m_gen);

	// Choose random positions for the intervals corresponding to the
	// other vertices. Compute them inductively.
	for (const node& v : neighs_root) {
		make_random_projective(m_T, root, v, m_rdata, m_gen);
	}

	// construct arrangement
	return internal::make_arrangement_intervals(m_T, root, m_rdata);
}

} // -- namespace generate
} // -- namespace lal
