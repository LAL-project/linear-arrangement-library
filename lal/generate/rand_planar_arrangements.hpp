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
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random generation of planar arrangements of a labeled rooted tree.
 *
 * A planar arrangement of a directed rooted tree is one in which the
 * root is not covered by any of the tree's edges and there are no edge
 * crossings.
 *
 * An example of usage of this class is
 * @code
 *		// given a rooted tree T
 *		lal::generate::rand_planar_arrgements Gen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		// given a rooted tree T
 *		lal::generate::rand_planar_arrangements Gen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const linear_arrangement arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class rand_planar_arrangements {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with a constant reference to a free tree.
	 * @param T Input free tree
	 * @param seed The seed used for the random generator. If the seed is 0 then
	 * a random seed is generated and used.
	 * @pre The object @e t must be a tree (see @ref lal::graphs::free_tree::is_tree).
	 */
	rand_planar_arrangements(const graphs::free_tree& T, uint64_t seed = 0) noexcept;

	/**
	 * @brief Constructor with rooted tree.
	 *
	 * This constructor transforms the input rooted tree into a free tree.
	 * @param T Input rooted tree.
	 * @param seed The seed used for the random generator. If the seed is 0 then
	 * a random seed is generated and used.
	 * @pre The object @e t must be a tree (see @ref lal::graphs::free_tree::is_tree).
	 */
	rand_planar_arrangements(const graphs::rooted_tree& T, uint64_t seed = 0) noexcept;

	/**
	 * @brief Default copy constructor.
	 * @param Gen Random planar arrangement generator.
	 */
	rand_planar_arrangements(const rand_planar_arrangements& Gen) = default;

	/**
	 * @brief Default move constructor.
	 * @param Gen Random planar arrangement generator.
	 */
	rand_planar_arrangements(rand_planar_arrangements&& Gen) = default;

	/// Default destructor.
	~rand_planar_arrangements() = default;

	/* GETTERS */

	/**
	 * @brief Make a random planar arrangement of a rooted tree.
	 * @returns A planar arrangement chosen uniformly at random
	 * chosen amongst all planar arrangements of @e t.
	 */
	linear_arrangement get_arrangement() noexcept;

	/// Returns a random planar arrangement.
	linear_arrangement yield_arrangement() noexcept {
		return get_arrangement();
	}

private:
	/**
	 * @brief A copy of a free tree.
	 *
	 * Used only when this class is constructed with a rooted tree.
	 */
	graphs::free_tree m_T_copy;
	/// The free tree of which we are making planar arrangements uniformly
	/// at random.
	const graphs::free_tree& m_T;

	/**
	 * @brief The random data for all vertices
	 *
	 * This is a member of the class to avoid its initialisation at
	 * every call to @ref get_arrangement.
	 */
	internal::data_array<std::vector<node>> m_rdata;

	/// The root chosen in the previous call to @ref get_arrangement or
	/// @ref yield_arrangement.
	node m_previous_root;

	/// Random number generator
	std::mt19937 m_gen;
};

} // -- namespace generate
} // -- namespace lal
