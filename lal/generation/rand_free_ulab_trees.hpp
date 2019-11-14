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
#include <map>

// lal includes
#include <lal/graphs/utree.hpp>
#include <lal/definitions.hpp>
#include <lal/numeric/integer.hpp>

namespace lal {
namespace generate {

/**
 * @brief Random free unlabelled tree generator.
 *
 * Generates uniformly at random free unlabelled trees.
 *
 * Every call to @ref make_rand_tree generates uniformly at random a free
 * unlabelled tree using the algorithm described in \cite Wilf1981a.
 * This is algorithm relies on the @e ranrut procedure (see \cite Nijenhuis1978a,
 * chapter 29).
 *
 * The users interested in generating trees of large size (of 100 vertices
 * or more) are recommended to take a look at @ref clear method.
 */
class rand_free_ulab_trees {
	public:
		/// Default constructor.
		rand_free_ulab_trees();
		/// Constructor with size of tree and seed for the random number generator.
		rand_free_ulab_trees(uint64_t n, uint64_t seed = 0);
		/// Default destructor.
		~rand_free_ulab_trees();

		/**
		 * @brief Sets the size of the unlabelled trees to generate.
		 *
		 * Enlarges the size of @ref m_Tn if more values are needed. The first
		 * 36 values are exracted from \cite OEIS_A000081. It also computes
		 * the values corresponding to \f$\alpha_{m,q}\f$, in @ref m_Amq, if
		 * needed. Initialises the random number generator.
		 * @param n Number of nodes of the tree.
		 * @param seed Integer value used to seed the random number generator.
		 */
		void init(uint64_t n, uint64_t seed = 0);

		/**
		 * @brief Generates uniformly at random a free unlabelled tree.
		 * @pre The generator must have been initialised.
		 * @return Returns an unlabelled tree.
		 */
		graphs::utree make_rand_tree();

		/**
		 * @brief Clears the memory occupied.
		 *
		 * In order to save computation time, this class has been designed
		 * to reuse memory when generating trees. For example, since it needs
		 * the values of well-known integer sequences (see attribute @ref m_Tn)
		 * that are costly to compute every time they are needed, they are
		 * stored in memory and reused over time.
		 *
		 * So, if the user wants to generate trees of 1000 vertices there will
		 * be too much memory occupied (and unused) if then this class is used
		 * to generate trees of 10 vertices. In cases like this it is
		 * recommended to clear the memory occupied.
		 *
		 * @post After calling this method, the contents of the attributes
		 * @ref m_Tn and @ref m_Amq are cleared. Attribute @ref m_Tn is then
		 * assigned the same 31 values that it is assigned when creating an
		 * object of this class.
		 */
		void clear();

	private:
		/// Number of nodes of the tree.
		uint64_t m_n = 0;
		/// Random number generator.
		std::mt19937 m_gen;
		/// Distribution of the numbers.
		std::uniform_real_distribution<double> m_unif;

		/// \f$t_n\f$ is the number of unlabelled rooted trees of \f$n\f$ vertices.
		std::vector<numeric::integer> m_Tn;

		/**
		 * @brief For each value of \f$q\f$ contains \f$\alpha_{m,q}\f$.
		 *
		 * \f$\alpha_{m,q}\f$ is he number of rooted forests of \f$m\f$
		 * vertices whose trees have at most \f$q\f$ vertices each. See
		 * \cite Wilf1981a.
		 *
		 * Since \f$m\f$ and \f$q\f$ are usually calculated as \f$m=n-1\f$
		 * and \f$q=(n - 1)/2\f$ there is only one value of \f$q\f$ for each
		 * \f$n\f$, so we do not need a matrix.
		 */
		std::map<uint64_t, std::vector<numeric::integer> > m_Amq;

		/**
		 * @brief List that encodes the tree.
		 *
		 * This list has @e n+1 values for @ref m_n vertices.
		 * A value of '0' indicates the root. A strictly positive value
		 * indicates the parent.
		 */
		std::vector<uint64_t> m_TREE;

	private:

		/// Initialiases @ref m_Tn with 31 values from the OEIS (see \cite OEIS_A000081).
		void init_T();
		/**
		 * @brief Computes all the values \f$t_i\f$ for \f$i \in [1,n]\f$.
		 *
		 * Here \f$n\f$ is @ref m_n. In case these values have already been
		 * calculated, this method does nothing.
		 */
		void compute_T();

		/**
		 * @brief Computes the values of @ref m_Amq.
		 *
		 * In case these values have already been
		 * calculated, this method does nothing.
		 * @param m Number of vertices of the forest.
		 * @param q Maximum number of vertices of each connected component of
		 * the forest.
		 */
		void compute_Amq_rec(uint64_t m, uint64_t q);
		/**
		 * @brief Computes the values \f$\alpha_{m,q}\f$ for \f$m=n-1, q=m/2\f$.
		 *
		 * Here \f$n\f$ is @ref m_n. Calls @ref compute_Amq_rec with
		 * parameter values \f$m=n - 1\f$ and \f$q = \lfloor m/2 \rfloor\f$.
		 */
		void compute_Amq();

		/**
		 * @brief Chooses uniformly at random a pair \f$(j,d)\f$, according
		 * to some probability.
		 *
		 * Probability of choosing \f$(j,d)\f$ is:
		 * \f$\frac{d \cdot t_{k - jd} \cdot t_d}{(k - 1)t_k}\f$.
		 * @param k Number of vertices.
		 * @param j Integer \f$j \ge 1\f$, \f$jd \le n\f$.
		 * @param d Integer \f$j \ge 1\f$, \f$jd \le n\f$.
		 */
		void choose_jd_from_T(uint64_t k, uint64_t& j, uint64_t& d);

		/**
		 * @brief Chooses uniformly at random a pair \f$(j,d)\f$, according
		 * to some probability.
		 *
		 * Probability of choosing \f$(j,d)\f$ is:
		 * \f$\frac{d \cdot \alpha_{m - jd, q} \cdot t_d}{m\alpha_{m, q}}\f$.
		 * Here, @e q is fixed to \f$(n - 1)/2\f$ where @e n is @ref m_n.
		 * @param m Amount of vertices.
		 * @param q Maximum amount of vertices per connected component.
		 * @param[out] j Integer \f$j \ge 1\f$, \f$jd \le n\f$.
		 * @param[out] d Integer \f$j \ge 1\f$, \f$jd \le n\f$.
		 */
		void choose_jd_from_Amq(uint64_t m, uint64_t q, uint64_t& j, uint64_t& d);

		/**
		 * @brief Generates uniformly at random a rooted unlabelled tree
		 * of @e k vertices.
		 * @param k Number of vertices of the rooted tree to generate.
		 * @param lr Index of the root of the last tree added.
		 * @param nt Index to @ref m_TREE where we have to place the new tree.
		 * @return Returns two new indices: the last root generated
		 * and where to store the next tree in @ref m_TREE.
		 */
		std::pair<uint64_t,uint64_t> ranrut(uint64_t k, uint64_t lr, uint64_t nt);
		/// Generates a tree of @e k vertices with two centroids.
		void bicenter(uint64_t k);
		/**
		 * @brief Generates uniformly at random a forest of @e m vertices.
		 *
		 * Makes a random forest of @e m vertices and stores it in @ref m_TREE.
		 * Each tree in the forest has at most @e q vertices.
		 * @param m Integer \f$m \ge 0\f$.
		 * @param q Integer \f$0 \le q \le m\f$.
		 * @param r_idx Index to @e roots indicating where store the root of
		 * the next generated tree.
		 * @param nt Index to @ref m_TREE indicating where to store the next
		 * tree.
		 * @param[out] roots Roots of the trees in the forest generated.
		 * @return Returns two new indices: where to store the next root in
		 * @e roots and where to store the next tree in @ref m_TREE.
		 */
		std::pair<uint64_t,uint64_t> forest
		(uint64_t m, uint64_t q, uint64_t r_idx,uint64_t nt, std::vector<uint64_t>& roots);
};

} // -- namespace generate
} // -- namespace lal
