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
#include <random>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/numeric/integer.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random unlabelled rooted tree generator.
 *
 * Generates uniformly at random rooted unlabelled trees.
 *
 * Every call to @ref get_tree generates rooted unlabelled trees uniformly at
 * random using the @e ranrut procedure (see \cite Nijenhuis1978a, chapter 29).
 *
 * Users interested in generating trees of large size (of 100 nodes
 * or more) are recommended to take a look at @ref clear method.
 *
 * The correct usage of this class is
 * @code
 *		rand_ulab_rooted_trees TreeGen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			lal::graphs::urtree T = TreeGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_ulab_rooted_trees {
	public:
		/**
		 * @brief Default constructor.
		 *
		 * When constructed this way, the class needs to be initialised.
		 * See @ref init(uint32_t, uint32_t).
		 */
		rand_ulab_rooted_trees() = default;
		/// Constructor with size of tree and seed for the random number generator.
		rand_ulab_rooted_trees(uint32_t n, uint32_t seed = 0);
		/// Default destructor.
		virtual ~rand_ulab_rooted_trees();

		/**
		 * @brief Sets the size of the unlabelled trees to generate.
		 *
		 * Initialises @ref m_rn with 31 values are extracted from
		 * \cite OEIS_A000081.
		 *
		 * Initialises the random number generator with @e seed. When @e seed
		 * is 0, a random seed is used.
		 * @param n Number of nodes of the tree.
		 * @param seed Integer value used to seed the random number generator.
		 */
		virtual void init(uint32_t n, uint32_t seed = 0);

		/**
		 * @brief Generates uniformly at random a free unlabelled tree.
		 * @return Returns an unlabelled rooted tree. The tree is rooted
		 * at vertex 0.
		 * @pre The generator must have been initialised.
		 */
		graphs::rooted_tree get_tree();

		/**
		 * @brief Clears the memory occupied.
		 *
		 * In order to save computation time, this class has been designed
		 * to reuse memory when generating trees. For example, since it needs
		 * the values of well-known integer sequences (see attribute @ref m_rn)
		 * that are costly to compute every time they are needed, they are
		 * stored in memory and reused over time.
		 *
		 * So, if the user wants to generate trees of 1000 nodes there will
		 * be too much memory occupied (and unused) if then this class is used
		 * to generate trees of 10 nodes. In cases like this it is
		 * recommended to clear the memory occupied.
		 *
		 * @post After calling this method, the contents of the attributes
		 * @ref m_rn are cleared. Attribute @ref m_rn is then assigned the same
		 * 31 values that it is assigned when creating an object of this class.
		 */
		void clear();

	protected:
		/// Number of nodes of the tree.
		uint32_t m_n = 0;
		/// Random number generator.
		std::mt19937 m_gen;
		/// Distribution of the numbers.
		std::uniform_real_distribution<double> m_unif;

		/**
		 * @brief The number of rooted unlabelled trees.
		 *
		 * Contains \f$r_n\f$ for \f$n\ge 0\f$.
		 */
		std::vector<numeric::integer> m_rn;

		/**
		 * @brief List that encodes the tree.
		 *
		 * This list has @e n+1 values for @ref m_n nodes.
		 * The first position is the root.
		 */
		uint32_t *m_tree = nullptr;

	protected:

		/**
		 * @brief Generates uniformly at random a rooted unlabelled tree of @e n nodes.
		 *
		 * The first call to this method should have @e lr = @ref m_n + 1.
		 * @param n Number of nodes of the rooted tree to generate.
		 * @param lr Pointer to the root of the last tree added.
		 *     @ref m_tree[@e lr] is the node that the root points to.
		 * @param nt Index to @ref m_tree where we have to place the new tree.
		 * @return Returns two indices: the index of the root of the last
		 * tree generated and where to store the next tree in @ref m_tree.
		 */
		std::pair<uint32_t,uint32_t> ranrut(uint32_t n, uint32_t lr, uint32_t nt);

		/// Initialiases @ref m_rn with 31 values from the OEIS (see \cite OEIS_A000081).
		void init_rn();
		/**
		 * @brief Computes all the values \f$t_i\f$ for \f$i \in [1,n]\f$.
		 *
		 * Here \f$n\f$ is @ref m_n. In case these values have already been
		 * calculated, this method does nothing.
		 */
		const numeric::integer& get_rn(uint32_t n);

		/**
		 * @brief Chooses uniformly at random a pair \f$(j,d)\f$, according
		 * to some probability.
		 *
		 * Probability of choosing \f$(j,d)\f$ is:
		 * \f$\frac{d \cdot t_{k - jd} \cdot t_d}{(k - 1)t_k}\f$.
		 * @param n Number of nodes.
		 * @returns Returns a pair of integers \f$(j,d)\f$ such that
		 * \f$j \ge 1\f$, \f$jd \le n\f$ and \f$j \ge 1\f$, \f$jd \le n\f$.
		 */
		std::pair<uint32_t,uint32_t> choose_jd_from_T(uint32_t n);
};

} // -- namespace generate
} // -- namespace lal
