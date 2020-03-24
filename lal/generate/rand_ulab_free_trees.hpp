/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <map>

// lal includes
#include <lal/graphs/utree.hpp>
#include <lal/definitions.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random unlabelled free tree generator.
 *
 * Generates uniformly at random free unlabelled trees.
 *
 * Every call to @ref make_rand_tree generates uniformly at random a free
 * unlabelled tree using the algorithm described in \cite Wilf1981a.
 * This is algorithm relies on the @e ranrut procedure (see \cite Nijenhuis1978a,
 * chapter 29) and runs in about the same time. The implementation of Wilf's
 * paper (see @ref make_rand_tree, @ref forest, and @ref bicenter) includes
 * the correction in Wilf's paper (see \cite Wilf1981a), as pointed out in
 * \cite GiacXcas_Manual (page 38).
 *
 * Users interested in generating trees of large size (of 100 nodes
 * or more) are recommended to take a look at @ref clear method.
 *
 * The correct usage of this class is
 * @code
 *		rand_ulab_free_trees TreeGen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			lal::graphs::utree T = TreeGen.make_rand_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_ulab_free_trees : public rand_ulab_rooted_trees {
	public:
		/**
		 * @brief Default constructor.
		 *
		 * When constructed this way, the class needs to be initialised.
		 * See @ref init(uint32_t, uint32_t).
		 */
		rand_ulab_free_trees();
		/// Constructor with size of tree and seed for the random number generator.
		rand_ulab_free_trees(uint32_t n, uint32_t seed = 0);
		/// Default destructor.
		~rand_ulab_free_trees();

		/**
		 * @brief Sets the size of the unlabelled trees to generate.
		 *
		 * Initialises @ref m_rn with 31 values extracted from
		 * \cite OEIS_A000081. It also initialises @ref m_fn with 31 values
		 * extracted from \cite OEIS_A000055.
		 *
		 * Initialises the random number generator with @e seed. When @e seed
		 * is 0, a random seed is used.
		 * @param n Number of nodes of the tree.
		 * @param seed Integer value used to seed the random number generator.
		 */
		void init(uint32_t n, uint32_t seed = 0);

		/**
		 * @brief Generates uniformly at random a free unlabelled tree.
		 *
		 * Includes the correction in Wilf's paper (see \cite Wilf1981a), as
		 * pointed out in \cite GiacXcas_Manual.
		 * @pre This class must have been initialised. See @ref init(uint32_t,uint32_t).
		 * @return Returns an unlabelled free tree.
		 */
		graphs::utree make_rand_tree();

		/**
		 * @brief Clears the memory occupied.
		 *
		 * In order to save computation time, this class has been designed
		 * to reuse memory when generating trees. For example, since it needs
		 * the values of well-known integer sequences (see attributes @ref m_rn
		 * and @ref m_alpha) that are costly to compute every time they are
		 * needed, they are stored in memory and reused over time.
		 *
		 * So, if the user wants to generate trees of 1000 nodes there will
		 * be too much memory occupied (and unused) if then this class is used
		 * to generate trees of 10 nodes. In cases like this it is
		 * recommended to clear the memory occupied.
		 *
		 * @post After calling this method, the contents of the attributes
		 * @ref m_rn, @ref m_fn and @ref m_alpha are cleared. Attributes
		 * @ref m_rn and @ref m_fn are then assigned the same 31 values that they
		 * are assigned when creating an object of this class.
		 */
		void clear();

	private:
		/**
		 * @brief Values \f$\alpha_{m,q}\f$.
		 *
		 * \f$\alpha_{m,q}\f$ is he number of rooted forests of \f$m\f$
		 * nodes whose trees have at most \f$q\f$ nodes each. See
		 * \cite Wilf1981a.
		 *
		 * Since \f$m\f$ and \f$q\f$ are usually calculated as \f$m=n-1\f$
		 * and \f$q=(n - 1)/2\f$ there is only one value of \f$q\f$ for each
		 * \f$n\f$, so we do not need a matrix.
		 */
		std::map<std::pair<uint32_t, uint32_t>, numeric::integer> m_alpha;

		/**
		 * @brief The number of free unlabelled trees.
		 *
		 * Contains \f$f_n\f$ for \f$n\ge 0\f$.
		 */
		std::vector<numeric::integer> m_fn;

	private:

		/**
		 * @brief Generates uniformly at random a forest of @e m nodes.
		 *
		 * Makes a random forest of @e m nodes and stores it in @ref m_tree.
		 * Each tree in the forest has at most @e q nodes.
		 * @param m Integer \f$m \ge 0\f$.
		 * @param q Integer \f$0 \le q \le m\f$.
		 * @param nt Index to @ref m_tree indicating where to store the next tree.
		 * @return Returns the position where to store the following
		 * trees/forests in @ref m_tree.
		 */
		uint32_t forest(uint32_t m, uint32_t q, uint32_t nt);

		/// Generates a tree of @e n nodes with two centroids.
		void bicenter(uint32_t n);

		/**
		 * @brief Computes and return the value \f$\alpha(m,q)\f$.
		 *
		 * Stores the calculated value in @ref m_alpha. In case the value has
		 * already been calculated, this method does nothing. See \cite Wilf1981a
		 * for details on \f$\alpha(m,q)\f$.
		 * @param m Number of nodes of the forest.
		 * @param q Maximum number of nodes of each connected component of
		 * the forest.
		 * @return Returns \f$\alpha(m,q)\f$
		 */
		const numeric::integer& get_alpha_mq(const uint32_t m, const uint32_t q);

		/// Initialiases @ref m_fn with 31 values from the OEIS (see \cite OEIS_A000055).
		void init_fn();

		/**
		 * @brief Computes and returns the value \f$f_n\f$.
		 *
		 * The value \f$f_n\f$ is the number of unlabelled free trees on \f$n\f$
		 * nodes. The method implements Otter's formula (see \cite Otter1948a).
		 * @param n Number of nodes of the tree.
		 * @return Returns \f$f_n\f$.
		 */
		const numeric::integer& get_fn(const uint32_t n);

		/**
		 * @brief Chooses uniformly at random a pair \f$(j,d)\f$, according
		 * to some probability.
		 *
		 * Probability of choosing \f$(j,d)\f$ is:
		 * \f$\frac{d \cdot \alpha_{m - jd, q} \cdot t_d}{m\alpha_{m, q}}\f$.
		 * Here, @e q is fixed to \f$(n - 1)/2\f$ where @e n is @ref m_n.
		 * @param m Amount of nodes.
		 * @param q Maximum amount of nodes per connected component.
		 * @returns Returns a pair of integers \f$(j,d)\f$ such that
		 * \f$j \ge 1\f$, \f$jd \le n\f$ and \f$j \ge 1\f$, \f$jd \le n\f$.
		 */
		std::pair<uint32_t,uint32_t>
		choose_jd_from_alpha(const uint32_t m, const uint32_t q);
};

} // -- namespace generate
} // -- namespace lal
