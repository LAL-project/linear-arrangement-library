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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/

#include <lal/generation/rand_rooted_lab_dir_trees.hpp>

// C++ includes
using namespace std;

namespace lal {
namespace generate {

rand_rooted_lab_dir_trees::rand_rooted_lab_dir_trees() : rand_free_lab_trees() { }

rand_rooted_lab_dir_trees::rand_rooted_lab_dir_trees(uint32_t n, uint32_t seed)
	: rand_free_lab_trees(n, seed)
{

}
rand_rooted_lab_dir_trees::~rand_rooted_lab_dir_trees() { }

void rand_rooted_lab_dir_trees::init(uint32_t n, uint32_t seed) {
	rand_free_lab_trees::init(n, seed);
}

rooted_directed_tree rand_rooted_lab_dir_trees::make_rand_tree() {
	ugraph t = rand_free_lab_trees::make_rand_tree();
	node r = m_unif(m_gen);
	return rooted_directed_tree(t, r);
}

} // -- namespace generate
} // -- namespace lal
