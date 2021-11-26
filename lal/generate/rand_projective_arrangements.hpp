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

#pragma once

// C++ includes
#include <random>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random generation of projective arrangements of a labeled rooted tree.
 *
 * A projective arrangement of a directed rooted tree is one in which the
 * root is not covered by any of the tree's edges and there are no edge
 * crossings.
 *
 * An example of usage of this class is
 * @code
 *		// given a rooted tree T
 *		lal::generate::rand_projective_arrgements Gen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		// given a rooted tree T
 *		lal::generate::rand_projective_arrangements Gen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const linear_arrangement arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class rand_projective_arrangements {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with tree
	 * @param rT Input rooted tree
	 * @param seed The seed used for the random generator. If the seed is 0 then
	 * a random seed is generated and used.
	 * @pre The object @e t must be a rooted tree (see
	 * @ref lal::graphs::rooted_tree::is_rooted_tree).
	 */
	rand_projective_arrangements(const graphs::rooted_tree& rT, uint64_t seed = 0) noexcept;
	/**
	 * @brief Default copy constructor.
	 * @param Gen Random projective arrangement generator.
	 */
	rand_projective_arrangements(const rand_projective_arrangements& Gen) = default;

	/**
	 * @brief Default move constructor.
	 * @param Gen Random projective arrangement generator.
	 */
	rand_projective_arrangements(rand_projective_arrangements&& Gen) = default;

	/// Default destructor.
	~rand_projective_arrangements() = default;

	/* GETTERS */

	/**
	 * @brief Make a random projective arrangement of a rooted tree.
	 * @returns A projective arrangement chosen uniformly at random
	 * chosen amongst all projective arrangements of @e t.
	 */
	linear_arrangement get_arrangement() noexcept;

	/// Returns a random projective arrangement.
	linear_arrangement yield_arrangement() noexcept {
		return get_arrangement();
	}

private:
	/// The rooted tree of which we are making projective arrangements uniformly at random.
	const graphs::rooted_tree& m_rT;

	/**
	 * @brief The random data for all vertices
	 *
	 * This is a member of the class to avoid its initialisation at
	 * every call to @ref get_arrangement.
	 */
	detail::data_array<detail::data_array<node>> m_rdata;

	/// Random number generator
	std::mt19937 m_gen;
};

} // -- namespace generate
} // -- namespace lal
