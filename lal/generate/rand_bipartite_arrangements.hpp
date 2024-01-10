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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/properties/bipartite_graph_colorability.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random generation of arrangements of any graph.
 *
 * This class generates bipartite linear arrangements uniformly at random
 * (see page @ref LAL_concepts__linear_arrangement__types for a definition of
 * bipartite arrangements). This class can be instantiated with a (bipartite)
 * graph, or with the coloring of one.
 *
 * A possible usage of this class is the following:
 * @code
 *		// given a tree T (or any other bipartite graph)
 *		lal::generate::rand_bipartite_arrangements Gen(T);
 *		for (int i = 0; i < 100; ++i) {
 *			const linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		// given a tree T (or any other bipartite graph)
 *		lal::properties::bipartite_graph_coloring c = lal::properties::coloring(T);
 *		lal::generate::rand_arrangements Gen(c);
 *		for (int i = 0; i < 100; ++i) {
 *			const linear_arrangement arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class rand_bipartite_arrangements {
public:

	/**
	 * @brief Constructor with graph.
	 *
	 * This constructor needs to calculate the bipartite coloring of the graph.
	 * @tparam graph_t Graph type. A subclass of lal::graphs::graph.
	 * @param g Input graph.
	 * @param seed Integer value to seed the random number generator.
	 * @pre The input graph @e g is bipartite.
	 */
	template <class graph_t>
	rand_bipartite_arrangements(const graph_t& g, uint64_t seed = 0) {
		static_assert(std::is_base_of_v<lal::graphs::graph, graph_t>);
		const auto c = properties::bipartite_coloring(g);
		init(c, seed);
	}

	/**
	 * @brief Default copy constructor.
	 * @param Gen Exhaustive bipartite arrangement generator.
	 */
	rand_bipartite_arrangements(const rand_bipartite_arrangements& Gen) = default;
	/**
	 * @brief Default move constructor.
	 * @param Gen Exhaustive bipartite arrangement generator.
	 */
	rand_bipartite_arrangements(rand_bipartite_arrangements&& Gen) = default;

	/**
	 * @brief Constructor with coloring.
	 * @param c Input coloring of a bipartite graph.
	 * @param seed Integer value to seed the random number generator.
	 */
	rand_bipartite_arrangements(
		const properties::bipartite_graph_coloring& c,
		uint64_t seed = 0
	)
	noexcept
	{
		init(c, seed);
	}

	/// Returns a linear arrangement constructed uniformly at random.
	const linear_arrangement& get_arrangement() noexcept;

	/// Returns a linear arrangement constructed uniformly at random.
	const linear_arrangement& yield_arrangement() noexcept {
		return get_arrangement();
	}

private:
	/**
	 * @brief Initializes this class.
	 *
	 * Initializes the arrangement (@ref m_arr) and the number of blue and red
	 * vertices (@ref m_n_blue and @ref m_n_red).
	 * @param c Coloring of the graph.
	 * @param seed Seed for the random number generator.
	 */
	void init(const properties::bipartite_graph_coloring& c, uint64_t seed) noexcept;

private:
	/**
	 * @brief The arrangement generated by this class.
	 *
	 * Actually, generated by the std::next_permutation algorithm.
	 */
	linear_arrangement m_arr;

	/// Has the end of the iteration been reached for blue vertices?
	bool m_reached_end_blue;
	/// Has the end of the iteration been reached for red vertices?
	bool m_reached_end_red;

	/// Number of blue vertices.
	std::size_t m_n_blue;
	/// Number of red vertices.
	std::size_t m_n_red;

	/// Random number generator
	std::mt19937 m_gen;
	/**
	 * @brief The arrangement generated by this class
	 *
	 * Actually, generated by the std::next_permutation algorithm.
	 */
};

} // -- namespace generate
} // -- namespace lal
