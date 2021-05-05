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
#include <lal/generate/tree_generator.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random generation of unlabelled rooted trees.
 *
 * <b>Users should refrain from using this class.</b> The generation of random
 * labelled trees should be done using the wrapper class @ref rand_ulab_rooted_trees.
 * This class, however, contains the actual code to generate labelled rooted trees
 * uniformly at random.
 *
 * Every call to @ref get_tree generates rooted unlabelled trees uniformly at
 * random using the @e ranrut procedure (see \cite Nijenhuis1978a, chapter 29).
 */
class _rand_ulab_rooted_trees {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator. If the seed is 0 then
	 * a random seed is generated and used.
	 */
	_rand_ulab_rooted_trees(uint32_t n, uint32_t seed = 0) noexcept;
	/**
	 * @brief Copy constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	_rand_ulab_rooted_trees(const _rand_ulab_rooted_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	_rand_ulab_rooted_trees(_rand_ulab_rooted_trees&& Gen) = default;
#endif
	/// Destructor.
	virtual ~_rand_ulab_rooted_trees() = default;

	/* GETTERS */

	/**
	 * @brief Generates uniformly at random a free unlabelled tree.
	 * @returns An unlabelled rooted tree. The tree is rooted
	 * at vertex 0.
	 */
	graphs::rooted_tree get_tree() noexcept;

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
	void clear() noexcept;

protected:
	/// Number of nodes of the tree.
	const uint32_t m_n;

	/// Random number generator.
	std::mt19937 m_gen;
	/// Distribution of the numbers.
	std::uniform_real_distribution<double> m_unif;

	/**
	 * @brief The number of unlabelled rooted trees.
	 *
	 * Contains \f$r_n\f$ for \f$n\ge 0\f$.
	 */
	std::vector<numeric::integer> m_rn;

	/**
	 * @brief The head vector of the tree under construction.
	 *
	 * This list has @e n values for @ref m_n nodes. The first position contains
	 * the root vertex.
	 *
	 * Do not use its actual type (@ref lal::head_vector) in an attempt to make
	 * memory usage a bit more efficient.
	 */
	internal::data_array<uint32_t> m_head_vector;

protected:
	/**
	 * @brief Sets the size of the unlabelled trees to generate.
	 *
	 * Initialises @ref m_rn with 31 values are extracted from \cite OEIS_A000081.
	 *
	 * Initialises the random number generator with @e seed. When @e seed
	 * is 0, a random seed is used.
	 * @param seed Integer value used to seed the random number generator.
	 */
	virtual void init(uint32_t seed = 0) noexcept;

	/**
	 * @brief Generates uniformly at random a rooted unlabelled tree of @e n nodes.
	 *
	 * The first call to this method should have @e lr = @ref m_n + 1.
	 * @param n Number of nodes of the rooted tree to generate.
	 * @param lr Pointer to the root of the last tree added.
	 *     @ref m_head_vector[@e lr] is the node that the root points to.
	 * @param nt Index to @ref m_head_vector where we have to place the new tree.
	 * @returns Two indices: the index of the root of the last
	 * tree generated and where to store the next tree in @ref m_head_vector.
	 */
	std::pair<uint32_t,uint32_t>
	ranrut(uint32_t n, uint32_t lr, uint32_t nt) noexcept;

	/// Initialiases @ref m_rn with 31 values from the OEIS (see \cite OEIS_A000081).
	void init_rn() noexcept;
	/**
	 * @brief Computes all the values \f$t_i\f$ for \f$i \in [1,n]\f$.
	 *
	 * Here \f$n\f$ is @ref m_n. In case these values have already been
	 * calculated, this method does nothing.
	 */
	const numeric::integer& get_rn(uint32_t n) noexcept;

	/**
	 * @brief Chooses uniformly at random a pair \f$(j,d)\f$, according
	 * to some probability.
	 *
	 * Probability of choosing \f$(j,d)\f$ is:
	 * \f$\frac{d \cdot t_{k - jd} \cdot t_d}{(k - 1)t_k}\f$.
	 * @param n Number of nodes.
	 * @returns A pair of integers \f$(j,d)\f$ such that
	 * \f$j \ge 1\f$, \f$jd \le n\f$ and \f$j \ge 1\f$, \f$jd \le n\f$.
	 */
	std::pair<uint32_t,uint32_t> choose_jd_from_T(uint32_t n) noexcept;
};

/**
 * @brief Uniformly random generation of unlabelled rooted trees.
 *
 * This is a wrapper class of @ref _rand_ulab_rooted_trees. Users should refrain
 * from using said class. However, users will find the implementation details
 * (as for algorithms and citations) in the documentation of said class.
 *
 * An example of usage of this class is
 * @code
 *		lal::generate::rand_ulab_rooted_trees Gen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::graphs::rooted_tree T = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_ulab_rooted_trees : public tree_generator<graphs::rooted_tree> {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator.
	 */
	rand_ulab_rooted_trees(uint32_t n, uint32_t seed = 0) noexcept
		: tree_generator<graphs::rooted_tree>(n), m_Gen(n, seed) { }
	/**
	 * @brief Copy constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	rand_ulab_rooted_trees(const rand_ulab_rooted_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	rand_ulab_rooted_trees(rand_ulab_rooted_trees&& Gen) = default;
#endif
	/// Default destructor.
	~rand_ulab_rooted_trees() noexcept = default;

protected:
	/**
	 * @brief Returns an unlabelled rooted tree chosen uniformly at random.
	 *
	 * See @ref _rand_ulab_rooted_trees::get_tree for details.
	 * @returns An unlabelled rooted tree chosen uniformly at random.
	 */
	inline graphs::rooted_tree __get_tree() noexcept { return m_Gen.get_tree(); }

protected:
	/// See @ref _rand_ulab_rooted_trees for details.
	_rand_ulab_rooted_trees m_Gen;
};

} // -- namespace generate
} // -- namespace lal
