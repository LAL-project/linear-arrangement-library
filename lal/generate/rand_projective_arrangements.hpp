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

#pragma once

// C++ includes
#include <random>

// lal includes
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Class to generate uniformly random projective arrangements of a rooted tree.
 *
 * A projective arrangement of a directed rooted tree is one in which the
 * root is not covered by any of the tree's edges and there are no edge
 * crossings.
 *
 * @code
 *		// given a tree T (of the appropriate type)
 *		lal::generate::rand_projective_arrgements ArrGen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::linear_arrangement arr = ArrGen.make_rand_arrgmnt();
 *			// ...
 *		}
 * @endcode
 */
class rand_projective_arrangements {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with tree
	 * @param rT Rooted tree
	 * @param seed The seed used for the random generator. If the seed is 0 then
	 * a random seed is generated and used.
	 * @pre The object @e t must be a rooted tree (see
	 * @ref graphs::rooted_tree::is_rooted_tree).
	 */
	rand_projective_arrangements(const graphs::rooted_tree& rT, uint32_t seed = 0) noexcept;
	/**
	 * @brief Default copy constructor.
	 * @param Gen Random projective arrangement generator.
	 */
	rand_projective_arrangements(const rand_projective_arrangements& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Default move constructor.
	 * @param Gen Random projective arrangement generator.
	 */
	rand_projective_arrangements(rand_projective_arrangements&& Gen) = default;
#endif
	/// Default destructor.
	~rand_projective_arrangements() = default;

	/* GETTERS */

	/**
	 * @brief Make a random projective arrangement of a rooted tree.
	 * @returns A projective arrangement chosen uniformly at random
	 * chosen amongst all projective arrangements of @e t.
	 */
	linear_arrangement make_rand_arrgmnt();

private:
	/// The rooted tree of we are making projective arrangements uniformly at random.
	const graphs::rooted_tree& m_rT;

	/**
	 * @brief The random data for all vertices
	 *
	 * This is a member of the class to avoid its initialisation at
	 * every call to @ref make_rand_arrgmnt.
	 */
	std::vector<std::vector<node>> rdata;

	/// Random number generator
	std::mt19937 m_gen;
};

} // -- namespace generate
} // -- namespace lal
