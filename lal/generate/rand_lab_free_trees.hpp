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
#include <lal/definitions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random generation of labelled free trees.
 *
 * <b>Users should refrain from using this class.</b> The generation of random
 * labelled trees should be done using the wrapper class @ref rand_lab_free_trees.
 * This class, however, contains the actual code to generate labelled free trees
 * uniformly at random.
 *
 * This class implements an algorithm that uses uniformly random Prüfer sequences
 * (see \cite Pruefer1918a). The construction of the free labelled tree uses the
 * algorithm in \cite Alonso1995a.
 */
class _rand_lab_free_trees {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator.
	 */
	_rand_lab_free_trees(uint64_t n, uint64_t seed = 0) noexcept;
	/**
	 * @brief Copy constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	_rand_lab_free_trees(const _rand_lab_free_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	_rand_lab_free_trees(_rand_lab_free_trees&& Gen) = default;
#endif
	/// Default destructor.
	virtual ~_rand_lab_free_trees() = default;

	/* GETTERS */

	/// Returns a labelled free tree chosen uniformly at random.
	graphs::free_tree get_tree() noexcept;

protected:
	/**
	 * @brief Sets the size of the labelled trees to generate.
	 *
	 * Initialises the random number generator.
	 * @param seed Integer value used to seed the random number generator.
	 */
	void init(uint64_t seed = 0) noexcept;

protected:
	/// Number of nodes of the tree.
	const uint64_t m_n;

	/// Random number generator.
	std::mt19937 m_gen;
	/// Distribution of the numbers.
	std::uniform_int_distribution<uint64_t> m_unif;
	/// Prüfer sequence.
	internal::data_array<uint64_t> m_Prufer_seq;
};

/**
 * @brief Uniformly random generation of labelled free trees.
 *
 * This is a wrapper class of @ref _rand_lab_free_trees. Users should refrain
 * from using said class. However, users will find the implementation details
 * (as for algorithms and citations) in the documentation of said class.
 *
 * An example of usage of this class is
 * @code
 *		lal::generate::rand_lab_free_trees Gen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::graphs::free_tree T = Gen.make_rand_tree();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		lal::generate::rand_lab_free_trees Gen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::graphs::free_tree T = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_lab_free_trees : public _tree_generator<graphs::free_tree> {
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
	rand_lab_free_trees(uint64_t n, uint64_t seed = 0) noexcept
		: _tree_generator<graphs::free_tree>(n), m_Gen(n, seed) { }
	/**
	 * @brief Copy constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	rand_lab_free_trees(const rand_lab_free_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	rand_lab_free_trees(rand_lab_free_trees&& Gen) = default;
#endif
	/// Default destructor.
	~rand_lab_free_trees() = default;

	inline graphs::free_tree yield_tree() noexcept {
		return get_tree();
	}

protected:
	/**
	 * @brief Returns a labelled free tree chosen uniformly at random.
	 *
	 * See @ref _rand_lab_free_trees::get_tree for details.
	 * @returns A labelled free tree chosen uniformly at random.
	 */
	inline graphs::free_tree __get_tree() noexcept { return m_Gen.get_tree(); }

protected:
	/// See @ref _rand_lab_free_trees.
	_rand_lab_free_trees m_Gen;
};

} // -- namespace generate
} // -- namespace lal
