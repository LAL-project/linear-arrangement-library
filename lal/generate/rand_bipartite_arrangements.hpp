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
 * @brief Random generation of arrangements of any bipartite graph.
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
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		// given a tree T (or any other bipartite graph)
 *		lal::properties::bipartite_graph_coloring c = lal::properties::coloring(T);
 *		lal::generate::rand_bipartite_arrangements Gen(c);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::linear_arrangement& arr = Gen.yield_arrangement();
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
	rand_bipartite_arrangements(const graph_t& g, const uint64_t seed = 0) noexcept {
		static_assert(std::is_base_of_v<graphs::graph, graph_t>);
		m_coloring = properties::bipartite_coloring(g);
		init(seed);
	}

	/**
	 * @brief Default copy constructor.
	 * @param Gen Exhaustive bipartite arrangement generator.
	 */
	rand_bipartite_arrangements(const rand_bipartite_arrangements& Gen) noexcept = default;
	/**
	 * @brief Default move constructor.
	 * @param Gen Exhaustive bipartite arrangement generator.
	 */
	rand_bipartite_arrangements(rand_bipartite_arrangements&& Gen) noexcept = default;

	/**
	 * @brief Constructor with coloring.
	 * @param c Input coloring of a bipartite graph.
	 * @param seed Integer value to seed the random number generator.
	 */
	rand_bipartite_arrangements
	(const properties::bipartite_graph_coloring& c, const uint64_t seed = 0)
	noexcept
	{
		m_coloring = c;
		init(seed);
	}
	/**
	 * @brief Constructor with coloring.
	 * @param c Input coloring of a bipartite graph.
	 * @param seed Integer value to seed the random number generator.
	 */
	rand_bipartite_arrangements
	(properties::bipartite_graph_coloring&& c, const uint64_t seed = 0)
	noexcept
	{
		m_coloring = std::move(c);
		init(seed);
	}

	/// Returns a linear arrangement constructed uniformly at random.
	[[nodiscard]] const linear_arrangement& get_arrangement() noexcept;

	/// Returns a linear arrangement constructed uniformly at random.
	[[nodiscard]] const linear_arrangement& yield_arrangement() noexcept {
		return get_arrangement();
	}

private:
	/**
	 * @brief Initializes this class.
	 *
	 * Initializes the arrangement (@ref m_arr) and the number of blue and red
	 * vertices (@ref m_n_blue and @ref m_n_red).
	 * @param seed Seed for the random number generator.
	 */
	void init(const uint64_t seed) noexcept;

	/**
	 * @brief Initializes this class.
	 *
	 * Initializes the arrangement by placing the red or blue vertices in the
	 * left side of the arrangement depending on the value of the parameter.
	 * @param red_first Place red vertices first if true.
	 */
	void init_arrangement(const bool red_first) noexcept;

private:
	/// Number of blue vertices.
	std::size_t m_n_blue;
	/// Number of red vertices.
	std::size_t m_n_red;

	/// Random number generator
	std::mt19937 m_gen;
	/**
	 * @brief Boolean values generator.
	 *
	 * To decide what color goes at the left half.
	 */
	std::bernoulli_distribution m_red_or_blue;

	/**
	 * @brief The arrangement generated by this class.
	 *
	 * Actually, generated by the std::next_permutation algorithm.
	 */
	linear_arrangement m_arr;

	/// Coloring of the bipartite graph.
	properties::bipartite_graph_coloring m_coloring;

	/// What color do we find in the left half?
	properties::bipartite_graph_coloring::color_t m_what_in_left;

private:
	/// Shortcut to blue color.
	static constexpr properties::bipartite_graph_coloring::color_t blue =
		properties::bipartite_graph_coloring::blue;
	/// Shortcut to red color.
	static constexpr properties::bipartite_graph_coloring::color_t red =
		properties::bipartite_graph_coloring::red;
};

} // -- namespace generate
} // -- namespace lal
