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
#include <vector>
#include <map>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/numeric/integer.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random generation of unlabelled free trees.
 *
 * <b>Users should refrain from using this class.</b> The generation of random
 * labelled trees should be done using the wrapper class @ref rand_ulab_free_trees.
 * This class, however, contains the actual code to generate labelled rooted trees
 * uniformly at random.
 *
 * Every call to @ref get_tree generates uniformly at random an unlabelled
 * free tree using the algorithm described in \cite Wilf1981a. This
 * algorithm relies on the @ref ranrut procedure (see \cite Nijenhuis1978a,
 * chapter 29) and runs in about the same time. The implementation of Wilf's
 * paper (see \cite Wilf1981a) in functions @ref get_tree, @ref forest,
 * and @ref bicenter includes the correction pointed out in
 * \cite GiacXcas_Manual (page 38).
 */
class _rand_ulab_free_trees : public _rand_ulab_rooted_trees {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator.
	 */
	_rand_ulab_free_trees(uint32_t n, uint32_t seed = 0) noexcept;
	/**
	 * @brief Copy constructor.
	 * @param Gen Random unlabelled free tree generator.
	 */
	_rand_ulab_free_trees(const _rand_ulab_free_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Random unlabelled free tree generator.
	 */
	_rand_ulab_free_trees(_rand_ulab_free_trees&& Gen) = default;
#endif
	/// Default destructor.
	~_rand_ulab_free_trees() = default;

	/* GETTERS */

	/**
	 * @brief Generates uniformly at random a free unlabelled tree.
	 *
	 * Includes the correction in Wilf's paper (see \cite Wilf1981a), as
	 * pointed out in \cite GiacXcas_Manual.
	 * @returns An unlabelled free tree.
	 */
	graphs::free_tree get_tree() noexcept;

protected:
	/**
	 * @brief Sets the size of the unlabelled trees to generate.
	 *
	 * Initialises @ref m_rn with 31 values extracted from
	 * \cite OEIS_A000081. It also initialises @ref m_fn with 31 values
	 * extracted from \cite OEIS_A000055.
	 *
	 * Initialises the random number generator with @e seed. When @e seed
	 * is 0, a random seed is used.
	 * @param seed Integer value used to seed the random number generator.
	 */
	void init(uint32_t seed = 0) noexcept;

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
	void clear() noexcept;

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
	 * Makes a random forest of @e m nodes and stores it in @ref m_head_vector.
	 * Each tree in the forest has at most @e q nodes.
	 * @param m Integer \f$m \ge 0\f$.
	 * @param q Integer \f$0 \le q \le m\f$.
	 * @param nt Index to @ref m_head_vector indicating where to store the next
	 * tree.
	 * @returns The position where to store the following
	 * trees/forests in @ref m_head_vector.
	 */
	uint32_t forest(uint32_t m, uint32_t q, uint32_t nt) noexcept;

	/// Generates a tree of @e n nodes with two centroids.
	void bicenter(uint32_t n) noexcept;

	/**
	 * @brief Computes and return the value \f$\alpha(m,q)\f$.
	 *
	 * Stores the calculated value in @ref m_alpha. In case the value has
	 * already been calculated, this method does nothing. See \cite Wilf1981a
	 * for details on \f$\alpha(m,q)\f$.
	 * @param m Number of nodes of the forest.
	 * @param q Maximum number of nodes of each connected component of
	 * the forest.
	 * @returns \f$\alpha(m,q)\f$
	 */
	const numeric::integer&
	get_alpha_mq(const uint32_t m, const uint32_t q) noexcept;

	/// Initialiases @ref m_fn with 31 values from the OEIS (see \cite OEIS_A000055).
	void init_fn() noexcept;

	/**
	 * @brief Computes and returns the value \f$f_n\f$.
	 *
	 * The value \f$f_n\f$ is the number of unlabelled free trees on \f$n\f$
	 * nodes. The method implements Otter's formula (see \cite Otter1948a).
	 * @param n Number of nodes of the tree.
	 * @returns \f$f_n\f$.
	 */
	const numeric::integer& get_fn(const uint32_t n) noexcept;

	/**
	 * @brief Chooses uniformly at random a pair \f$(j,d)\f$, according
	 * to some probability.
	 *
	 * Probability of choosing \f$(j,d)\f$ is:
	 * \f$\frac{d \cdot \alpha_{m - jd, q} \cdot t_d}{m\alpha_{m, q}}\f$.
	 * Here, @e q is fixed to \f$(n - 1)/2\f$ where @e n is @ref m_n.
	 * @param m Amount of nodes.
	 * @param q Maximum amount of nodes per connected component.
	 * @returns A pair of integers \f$(j,d)\f$ such that
	 * \f$j \ge 1\f$, \f$jd \le n\f$ and \f$j \ge 1\f$, \f$jd \le n\f$.
	 */
	std::pair<uint32_t,uint32_t>
	choose_jd_from_alpha(const uint32_t m, const uint32_t q) noexcept;
};

/**
 * @brief Uniformly random generation of unlabelled free trees.
 *
 * This is a wrapper class of @ref _rand_ulab_free_trees. Users should refrain
 * from using said class. However, users will find the implementation details
 * (as for algorithms and citations) in the documentation of said class.
 *
 * An example of usage of this class is
 * @code
 *		lal::generate::rand_ulab_free_trees Gen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::graphs::free_tree T = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_ulab_free_trees : public _tree_generator<graphs::free_tree> {
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
	rand_ulab_free_trees(uint32_t n, uint32_t seed = 0) noexcept
		: _tree_generator<graphs::free_tree>(n), m_Gen(n, seed) { }
	/**
	 * @brief Copy constructor.
	 * @param Gen Random unlabelled free tree generator.
	 */
	rand_ulab_free_trees(const rand_ulab_free_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Random unlabelled free tree generator.
	 */
	rand_ulab_free_trees(rand_ulab_free_trees&& Gen) = default;
#endif
	/// Default destructor.
	~rand_ulab_free_trees() = default;

protected:
	/**
	 * @brief Returns an unlabelled free tree chosen uniformly at random.
	 *
	 * See @ref _rand_ulab_free_trees::get_tree for details.
	 * @returns An unlabelled free tree chosen uniformly at random.
	 */
	inline graphs::free_tree __get_tree() noexcept { return m_Gen.get_tree(); }

protected:
	/// See @ref _rand_ulab_free_trees.
	_rand_ulab_free_trees m_Gen;
};

} // -- namespace generate
} // -- namespace lal
