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

#pragma once

// C++ includes
#include <vector>
#include <random>

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/urtree.hpp>
#include <lal/numeric/integer.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random rooted unlabelled tree generator.
 *
 * Generates uniformly at random rooted unlabelled trees.
 *
 * Every call to @ref make_rand_tree generates rooted unlabelled trees
 * uniformly at random using the @e ranrut procedure (see \cite Nijenhuis1978a,
 * chapter 29).
 *
 * Users interested in generating trees of large size (of 100 vertices
 * or more) are recommended to take a look at @ref clear method.
 */
class rand_rooted_ulab_trees {
	public:
		/**
		 * @brief Default constructor.
		 *
		 * When constructed this way, the class needs to be initialised.
		 * See @ref init(uint32_t, uint32_t).
		 */
		rand_rooted_ulab_trees();
		/// Constructor with size of tree and seed for the random number generator.
		rand_rooted_ulab_trees(uint32_t n, uint32_t seed = 0);
		/// Default destructor.
		virtual ~rand_rooted_ulab_trees();

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
		 * @pre The generator must have been initialised.
		 * @return Returns an unlabelled tree.
		 */
		std::vector<uint32_t> make_rand_tree();

		/**
		 * @brief Clears the memory occupied.
		 *
		 * In order to save computation time, this class has been designed
		 * to reuse memory when generating trees. For example, since it needs
		 * the values of well-known integer sequences (see attribute @ref m_rn)
		 * that are costly to compute every time they are needed, they are
		 * stored in memory and reused over time.
		 *
		 * So, if the user wants to generate trees of 1000 vertices there will
		 * be too much memory occupied (and unused) if then this class is used
		 * to generate trees of 10 vertices. In cases like this it is
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
		 * This list has @e n+1 values for @ref m_n vertices.
		 * The first position is the root.
		 */
		std::vector<uint32_t> m_tree;

	protected:

		/**
		 * @brief Generates uniformly at random a rooted unlabelled tree of @e n vertices.
		 *
		 * The first call to this method should have @e lr = @ref m_n + 1.
		 * @param n Number of vertices of the rooted tree to generate.
		 * @param lr Root (vertex) of the last tree added. @ref m_tree[@e lr]
		 * is the vertex that the root points to.
		 * @param nt Index to @ref m_tree where we have to place the new tree.
		 * @return Returns two indices: the index of the root of the last
		 * tree generated and where to store the next tree in @ref m_tree.
		 */
		std::pair<uint32_t,uint32_t> ranrut
		(uint32_t n, uint32_t lr, uint32_t nt, const std::string& tab = "");

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
		 * @param n Number of vertices.
		 * @returns Returns a pair of integers \f$(j,d)\f$ such that
		 * \f$j \ge 1\f$, \f$jd \le n\f$ and \f$j \ge 1\f$, \f$jd \le n\f$.
		 */
		std::pair<uint32_t,uint32_t> choose_jd_from_T(uint32_t n);
};

} // -- namespace generate
} // -- namespace lal
