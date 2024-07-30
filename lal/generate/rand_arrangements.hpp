/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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

#pragma once

// C++ includes
#include <random>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/graph.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random generation of arrangements of any graph.
 *
 * This class generates linear arrangements uniformly at random. Unlike other
 * random generators (e.g., @ref lal::generate::rand_projective_arrangements)
 * the arrangements are not generated from a graph since the graph structure
 * is not required at all.
 *
 * This class is a simple wrapper over C++'s std::shuffle algorithm.
 *
 * A possible usage of this class is the following:
 * @code
 *		// given a tree T (or any other graph)
 *		lal::generate::rand_arrangements Gen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		// given a tree T (or any other graph)
 *		lal::generate::rand_arrangements Gen(T.get_num_nodes());
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::linear_arrangement& arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class rand_arrangements {
public:
	/**
	 * @brief Constructor with graph.
	 * @param g Input graph. Only its number of vertices is used.
	 * @param seed Seed of the random number generator. When 0, a random seed
	 * is used.
	 */
	rand_arrangements(const graphs::graph& g, uint64_t seed = 0) noexcept :
		rand_arrangements(g.get_num_nodes(), seed)
	{ }

	/**
	 * @brief Default copy constructor.
	 * @param Gen Random arrangement generator.
	 */
	rand_arrangements(const rand_arrangements& Gen) noexcept = default;
	/**
	 * @brief Default move constructor.
	 * @param Gen Random arrangement generator.
	 */
	rand_arrangements(rand_arrangements&& Gen) noexcept = default;

	/**
	 * @brief Constructor with number of vertices.
	 * @param n Number of vertices of the arrangements.
	 * @param seed Seed of the random number generator. When 0, a random seed
	 * is used.
	 */
	rand_arrangements(uint64_t n, uint64_t seed = 0) noexcept :
		m_n(n),
		m_arr(linear_arrangement(m_n))
	{
		if (seed == 0) {
			std::random_device rd;
			m_gen = std::mt19937(rd());
		}
		else {
			m_gen = std::mt19937(seed);
		}
		m_arr.identity();
	}

	/// Returns a linear arrangement constructed uniformly at random.
	[[nodiscard]] const linear_arrangement& get_arrangement() noexcept;

	/// Returns a linear arrangement constructed uniformly at random.
	[[nodiscard]] const linear_arrangement& yield_arrangement() noexcept {
		return get_arrangement();
	}

private:
	/// Number of vertices
	const uint64_t m_n;
	/// Random number generator
	std::mt19937 m_gen;
	/**
	 * @brief The arrangement generated by this class
	 *
	 * Actually, generated by the std::next_permutation algorithm.
	 */
	linear_arrangement m_arr;
};

} // -- namespace generate
} // -- namespace lal
