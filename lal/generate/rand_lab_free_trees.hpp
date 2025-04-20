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
#if defined LAL_REGISTER_BIBLIOGRAPHY
#include <lal/bibliography.hpp>
#endif
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random selection of labelled free trees.
 *
 * <b>Users should refrain from using this class.</b> The generation of random
 * labelled trees should be done using the wrapper class @ref rand_lab_free_trees.
 * This class, however, contains the actual code to generate labelled free trees
 * uniformly at random.
 *
 * This class implements an algorithm that uses uniformly random Prüfer sequences
 * (see \cite Pruefer1918a). The construction of the free labelled tree is done
 * in \f$O(n)\f$.
 */
class _rand_lab_free_trees {
public:

	/* CONSTRUCTORS */

	/// Default constructor
	_rand_lab_free_trees() noexcept { }

	/**
	 * @brief Constructor with size of tree and seed for the random number
	 * generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator.
	 */
	_rand_lab_free_trees(const uint64_t n, const uint64_t seed = 0) noexcept
	{
		init(n, seed);
	}

	/**
	 * @brief Copy constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	_rand_lab_free_trees(const _rand_lab_free_trees& Gen) noexcept = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	_rand_lab_free_trees(_rand_lab_free_trees&& Gen) noexcept = default;

	/// Default destructor.
	~_rand_lab_free_trees() = default;

	/// Copy assignment operator.
	_rand_lab_free_trees& operator= (const _rand_lab_free_trees& g
	) noexcept = default;
	/// Move assignment operator.
	_rand_lab_free_trees& operator= (_rand_lab_free_trees&& g
	) noexcept = default;

	/* INITIALIZE */

	/**
	 * @brief Initializes the generator with the number of nodes and a seed.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator. If the seed is 0
	 * then a random seed is generated and used.
	 */
	void init(const uint64_t n, const uint64_t seed = 0) noexcept
	{
#if defined LAL_REGISTER_BIBLIOGRAPHY
		bibliography::register_entry(bibliography::entries::Pruefer1918a);
#endif

		m_n = n;
		m_Prufer_seq.resize(m_n <= 2 ? 2 : m_n - 2);

		if (seed == 0) {
			std::random_device rd;
			m_gen = std::mt19937(rd());
		}
		else {
			m_gen = std::mt19937(seed);
		}
		m_unif = std::uniform_int_distribution<uint64_t>(0, m_n - 1);
	}

	/**
	 * @brief Clears the memory used.
	 * @post Method @ref init must be called after every call to @ref clear.
	 */
	void clear() noexcept
	{
		m_Prufer_seq.clear();
	}

	/* GETTERS */

	/// Returns a labelled free tree chosen uniformly at random.
	[[nodiscard]] graphs::free_tree get_tree() noexcept;

protected:

	/// Number of nodes of the tree.
	uint64_t m_n;

	/// Random number generator.
	std::mt19937 m_gen;
	/// Distribution of the numbers.
	std::uniform_int_distribution<uint64_t> m_unif;
	/// Prüfer sequence.
	detail::array<uint64_t> m_Prufer_seq;
};

/**
 * @brief Uniformly random selection of labelled free trees.
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

	/// Empty constructor
	rand_lab_free_trees() noexcept
		: _tree_generator<graphs::free_tree>()
	{ }

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator. If the seed is 0 then
	 * a random seed is generated and used.
	 */
	rand_lab_free_trees(const uint64_t n, const uint64_t seed = 0) noexcept
		: _tree_generator<graphs::free_tree>(n),
		  m_Gen(n, seed)
	{ }
	/**
	 * @brief Copy constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	rand_lab_free_trees(const rand_lab_free_trees& Gen) = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Random labelled free tree generator.
	 */
	rand_lab_free_trees(rand_lab_free_trees&& Gen) = default;

	/// Default destructor.
	~rand_lab_free_trees() = default;

	/// Copy assignment operator.
	rand_lab_free_trees& operator= (const rand_lab_free_trees& g
	) noexcept = default;
	/// Move assignment operator.
	rand_lab_free_trees& operator= (rand_lab_free_trees&& g) noexcept = default;

	/**
	 * @brief Initializes the generator with the number of nodes and a seed.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator. If the seed is 0
	 * then a random seed is generated and used.
	 */
	void init(const uint64_t n, const uint64_t seed = 0) noexcept
	{
		_tree_generator::init(n);
		m_Gen.init(n, seed);
	}

	/// Clear the memory used by the generator.
	void clear() noexcept
	{
		_tree_generator::clear();
		m_Gen.clear();
	}

	[[nodiscard]] graphs::free_tree yield_tree() noexcept
	{
		return get_tree();
	}

protected:

	/**
	 * @brief Returns a labelled free tree chosen uniformly at random.
	 *
	 * See @ref _rand_lab_free_trees::get_tree for details.
	 * @returns A labelled free tree chosen uniformly at random.
	 */
	[[nodiscard]] graphs::free_tree _get_tree() noexcept
	{
		return m_Gen.get_tree();
	}

protected:

	/// See @ref _rand_lab_free_trees.
	_rand_lab_free_trees m_Gen;
};

} // namespace generate
} // namespace lal
