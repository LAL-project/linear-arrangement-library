/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <vector>
#include <random>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random labelled free tree generator.
 *
 * Generates uniformly at random labelled free trees. The algorithm uses
 * uniformly random Prüfer sequences (see \cite Pruefer1918a). The construction
 * of the free labelled tree uses the algorithm in \cite Alonso1995a.
 *
 * The correct usage of this class is
 * @code
 *		rand_lab_free_trees TreeGen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			lal::graphs::utree T = TreeGen.make_rand_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_lab_free_trees {
	public:
		/**
		 * @brief Default constructor.
		 *
		 * When constructed this way, the class needs to be initialised.
		 * See @ref init(uint32_t, uint32_t).
		 */
		rand_lab_free_trees() = default;
		/// Constructor with size of tree and seed for the random number generator.
		rand_lab_free_trees(uint32_t n, uint32_t seed = 0);
		/// Default Destructor.
		virtual ~rand_lab_free_trees() = default;

		/**
		 * @brief Sets the size of the labelled trees to generate.
		 *
		 * Initialises the random number generator.
		 * @param n Number of nodes of the tree.
		 * @param seed Integer value used to seed the random number generator.
		 */
		void init(uint32_t n, uint32_t seed = 0);

		/**
		 * @brief Generates uniformly at random a free labelled tree.
		 * @pre The generator must have been initialised.
		 * @return Returns a labelled tree.
		 */
		graphs::free_tree make_rand_tree();

	protected:
		/// Number of nodes of the tree.
		uint32_t m_n;
		/// Random number generator.
		std::mt19937 m_gen;
		/// Distribution of the numbers.
		std::uniform_int_distribution<uint32_t> m_unif;
		/// Prüfer sequence.
		std::vector<uint32_t> m_seq;
};

} // -- namespace generate
} // -- namespace lal
