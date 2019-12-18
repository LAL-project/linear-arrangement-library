/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#include <lal/linarr/C.hpp>

// C++ includes
using namespace std;

namespace lal {
using namespace graphs;

namespace linarr {

uint32_t n_crossings
(const ugraph& g, const LINARR& pi, const algorithms_crossings& A) {
	switch (A) {
	case algorithms_crossings::brute_force:
		return __n_crossings_brute_force(g, pi);
	case algorithms_crossings::dynamic_programming:
		return __n_crossings_dyn_prog(g, pi);
	case algorithms_crossings::ladder:
		return __n_crossings_ladder(g, pi);
	case algorithms_crossings::stack_based:
		return __n_crossings_stack_based(g, pi);
	}

	// wrong value of enumeration
	return g.n_edges()*g.n_edges();
}

vector<uint32_t> n_crossings_list
(const ugraph& g, const vector<LINARR>& pis, const algorithms_crossings& A)
{
	switch (A) {
	case algorithms_crossings::brute_force:
		return __n_crossings_brute_force_list(g, pis);
	case algorithms_crossings::dynamic_programming:
		return __n_crossings_dyn_prog_list(g, pis);
	case algorithms_crossings::ladder:
		return __n_crossings_ladder_list(g, pis);
	case algorithms_crossings::stack_based:
		return __n_crossings_stack_based_list(g, pis);
	}

	// wrong value of enumeration
	return vector<uint32_t>(pis.size(), g.n_edges()*g.n_edges());
}

} // -- namespace linarr
} // -- namespace lal
