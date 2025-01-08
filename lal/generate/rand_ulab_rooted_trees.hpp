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
#include <tuple>

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/numeric/integer.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Uniformly random selection of unlabelled rooted trees.
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

	/// Empty constructor
	_rand_ulab_rooted_trees() noexcept {
		// add the initial values to m_rn
		init_rn();
	}

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random number generator. If the seed is
	 * 0 then a random seed is generated and used.
	 */
	_rand_ulab_rooted_trees(const uint64_t n, const uint64_t seed = 0) noexcept :
		_rand_ulab_rooted_trees()
	{
		// add the initial values to m_rn
		init_rn();

		init(n, seed);
	}

	/**
	 * @brief Copy constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	_rand_ulab_rooted_trees(const _rand_ulab_rooted_trees& Gen) = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	_rand_ulab_rooted_trees(_rand_ulab_rooted_trees&& Gen) = default;

	/// Destructor.
	virtual ~_rand_ulab_rooted_trees() = default;

	/// Copy assignment operator.
	_rand_ulab_rooted_trees& operator= (const _rand_ulab_rooted_trees& g) noexcept = default;
	/// Move assignment operator.
	_rand_ulab_rooted_trees& operator= (_rand_ulab_rooted_trees&& g) noexcept = default;

	/* INITIALIZE */

	/**
	 * @brief Sets the size of the unlabelled trees to generate.
	 *
	 * Adds the remaining necessary values to @ref m_rn..
	 *
	 * Initializes the random number generator with @e seed. When @e seed
	 * is 0, a random seed is used.
	 * @param n Number of vertices.
	 * @param seed Integer value used to seed the random number generator.
	 */
	void init(const uint64_t n, const uint64_t seed = 0) noexcept {
		// setup memory
		m_n = n;
		m_head_vector.resize(m_n);

		if (m_n <= 1) { return; }

		// initialize the random number generators
		if (seed == 0) {
			std::random_device rd;
			m_gen = std::mt19937(rd());
		}
		else {
			m_gen = std::mt19937(seed);
		}
		m_unif = std::uniform_real_distribution<double>(0, 1);

		// force the addition of the necessary values for m_rn
		std::ignore = get_rn(n);
	}

	/**
	 * @brief Clears the memory used.
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
	 * values that it is assigned when creating an object of this class.
	 * @post Method @ref init must be called after every call to @ref clear.
	 */
	void clear() noexcept {
		// clear the values in m_rn
		m_rn.clear();
		// add the initial values to m_rn
		init_rn();
		// clear the other memory
		m_head_vector.clear();
	}

	/* GETTERS */

	/**
	 * @brief Generates uniformly at random a free unlabelled tree.
	 * @returns An unlabelled rooted tree. The tree is rooted
	 * at vertex 0.
	 */
	[[nodiscard]] graphs::rooted_tree get_tree() noexcept;

protected:
	/// Number of nodes of the tree.
	uint64_t m_n;

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
	 * @brief The number of unlabelled rooted trees times number of vertices.
	 *
	 * Contains \f$r_n \cdot n\f$ for \f$n\ge 0\f$.
	 */
	std::vector<numeric::integer> m_rn_times_n;
	/**
	 * @brief The number of unlabelled rooted trees times number of vertices minus 1.
	 *
	 * Contains \f$r_n \cdot (n - 1)\f$ for \f$n\ge 0\f$.
	 */
	std::vector<numeric::integer> m_rn_times_n_minus_1;

	/**
	 * @brief The head vector of the tree under construction.
	 *
	 * The first position always contains the root vertex. The parent of vertex
	 * @e u is located at @e m_head_vector[u], but the value is an index from
	 * 0 to \f$n - 1\f$, both included.
	 */
	head_vector m_head_vector;

protected:

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
	[[nodiscard]] std::pair<uint64_t,uint64_t> ranrut
	(
		const uint64_t n,
		const uint64_t lr,
		uint64_t nt
	)
	noexcept;

	/// Initialiases @ref m_rn with values from the OEIS (see \cite OEIS_A000081).
	void init_rn() noexcept {
		// from the OEIS: https://oeis.org/A000081
		m_rn =
		std::vector<numeric::integer>{
			0ull,
			1ull,
			1ull,
			2ull,
			4ull,
			9ull,
			20ull,
			48ull,
			115ull,
			286ull,
			719ull,
			1842ull,
			4766ull,
			12486ull,
			32973ull,
			87811ull,
			235381ull,
			634847ull,
			1721159ull,
			4688676ull,
			12826228ull,
			35221832ull,
			97055181ull,
			268282855ull,
			743724984ull,
			2067174645ull,
			5759636510ull,
			16083734329ull,
			45007066269ull,
			126186554308ull,
			354426847597ull
		};
		m_rn_times_n.resize(m_rn.size());
		m_rn_times_n[0] = 0;
		m_rn_times_n_minus_1.resize(m_rn.size());
		m_rn_times_n_minus_1[0] = 0;
		for (uint64_t n = 1; n < m_rn.size(); ++n) {
			m_rn_times_n[n] = m_rn[n]*n;
			m_rn_times_n_minus_1[n] = m_rn_times_n[n] - m_rn[n];
		}
	}

	/**
	 * @brief Computes all the values \f$t_i\f$ for \f$i \in [1,n]\f$.
	 *
	 * Here \f$n\f$ is @ref m_n. In case these values have already been
	 * calculated, this method does nothing.
	 */
	[[nodiscard]] const numeric::integer& get_rn(const uint64_t n) noexcept;

	/// Returns whether or not the value \f$r_n\f$ (@ref m_rn) has been computed.
	[[nodiscard]] bool has_rn(const uint64_t n) const noexcept { return m_rn.size() >= n + 1; }

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
	[[nodiscard]] std::pair<uint64_t,uint64_t> choose_jd_from_T(const uint64_t n) noexcept;
};

/**
 * @brief Uniformly random selection of unlabelled rooted trees.
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
 * Equivalently,
 * @code
 *		lal::generate::rand_ulab_rooted_trees Gen(n);
 *		for (int i = 0; i < 100; ++i) {
 *			const lal::graphs::rooted_tree T = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 */
class rand_ulab_rooted_trees : public _tree_generator<graphs::rooted_tree> {
public:
	/* CONSTRUCTORS */

	/// Empty constructor
	rand_ulab_rooted_trees() noexcept { }

	/**
	 * @brief Constructor with size of tree and seed for the random number generator.
	 *
	 * In case the seed given is '0', a random seed will be generated.
	 * @param n Number of nodes.
	 * @param seed The seed used for the random generator. If @e seed is 0 then
	 * a random seed is used.
	 */
	rand_ulab_rooted_trees(const uint64_t n, const uint64_t seed = 0) noexcept :
		_tree_generator<graphs::rooted_tree>(n), m_Gen(n, seed) { }
	/**
	 * @brief Copy constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	rand_ulab_rooted_trees(const rand_ulab_rooted_trees& Gen) noexcept = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Random unlabelled rooted tree generator.
	 */
	rand_ulab_rooted_trees(rand_ulab_rooted_trees&& Gen) noexcept = default;

	/// Default destructor.
	~rand_ulab_rooted_trees() noexcept = default;

	/// Copy assignment operator.
	rand_ulab_rooted_trees& operator= (const rand_ulab_rooted_trees& g) noexcept = default;
	/// Move assignment operator.
	rand_ulab_rooted_trees& operator= (rand_ulab_rooted_trees&& g) noexcept = default;

	/**
	 * @brief Initialize the generator.
	 * @param n Number of vertices
	 * @param seed The seed used for the random generator. If @e seed is 0 then
	 * a random seed is used.
	 */
	void init(const uint64_t n, const uint64_t seed = 0) noexcept {
		_tree_generator::init(n);
		m_Gen.init(n, seed);
	}

	/// Clear the memory used by the generator.
	void clear() noexcept {
		_tree_generator::clear();
		m_Gen.clear();
	}

	[[nodiscard]] graphs::rooted_tree yield_tree() noexcept {
		return get_tree();
	}

protected:
	/**
	 * @brief Returns an unlabelled rooted tree chosen uniformly at random.
	 *
	 * See @ref _rand_ulab_rooted_trees::get_tree for details.
	 * @returns An unlabelled rooted tree chosen uniformly at random.
	 */
	[[nodiscard]] graphs::rooted_tree __get_tree() noexcept { return m_Gen.get_tree(); }

protected:
	/// See @ref _rand_ulab_rooted_trees for details.
	_rand_ulab_rooted_trees m_Gen;
};

} // -- namespace generate
} // -- namespace lal
