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

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of unlabelled free trees.
 *
 * Generates all the unlabelled free bistar trees of a given number of nodes.
 * This enumeration always includes the @ref lal::graphs::tree_type::star tree
 * of \f$n\f$ vertices as the first tree.
 *
 * In order to use this class, users must provide the size \f$n\f$ of the tree
 * (number of nodes) in the constructor. Trees are generated internally, i.e.,
 * trees are encoded in the internal state of the generator. Said state is
 * updated using method @ref next(), which updates it to encode the next tree in
 * the generation. In order to retrieve the tree, use method @ref get_tree().
 * Upon initialisation, the generator encodes the first tree, which has to be
 * retrieved using method @ref get_tree().
 *
 * All the unlabelled free trees will have been generated when method @ref end()
 * returns true. At this point, method @ref get_tree() will always construct the
 * last tree in the enumeration, whichever tree it is. In order to restart the
 * generation of these trees, call method @ref reset(). It is allowed to call
 * this method at any time.
 *
 * A possible usage of this class is the following:
 * @code
 *		lal::generate::all_ulab_free_bistar_trees Gen(n);
 *		while (not Gen.end()) {
 *			const lal::graphs::free_tree t = Gen.get_tree();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (lal::generate::all_ulab_free_bistar_trees Gen(n); not Gen.end(); Gen.next()) {
 *			const lal::graphs::free_tree t = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		lal::generate::all_ulab_free_bistar_trees Gen(n);
 *		while (not Gen.end()) {
 *			const lal::graphs::free_tree t = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_ulab_free_bistar_trees : public _tree_generator<graphs::free_tree> {
public:

	/// Empty constructor.
	all_ulab_free_bistar_trees() noexcept
		: _tree_generator<graphs::free_tree>()
	{ }

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	all_ulab_free_bistar_trees(const uint64_t n) noexcept
	{
		init(n);
	}
	/**
	 * @brief Copy constructor.
	 * @param Gen Exhaustive unlabelled free tree generator.
	 */
	all_ulab_free_bistar_trees(const all_ulab_free_bistar_trees& Gen
	) noexcept = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Exhaustive unlabelled free tree generator.
	 */
	all_ulab_free_bistar_trees(all_ulab_free_bistar_trees&& Gen
	) noexcept = default;

	/// Default destructor.
	~all_ulab_free_bistar_trees() noexcept = default;

	/// Copy assignment operator.
	all_ulab_free_bistar_trees& operator= (const all_ulab_free_bistar_trees& g
	) noexcept = default;
	/// Move assignment operator.
	all_ulab_free_bistar_trees& operator= (all_ulab_free_bistar_trees&& g
	) noexcept = default;

	/* INITIALIZE */

	/**
	 * @brief Initializes the generator with a given number of vertices.
	 * @param n Number of vertices.
	 */
	void init(const uint64_t n) noexcept
	{
		_tree_generator::init(n);
		m_size = 0;
		reset();
	}

	/**
	 * @brief Clears the memory used.
	 * @post Method @ref init must be called after every call to @ref clear.
	 */
	void clear() noexcept
	{
		_tree_generator::clear();
	}

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	[[nodiscard]] bool end() const noexcept
	{
		return m_n == 0 ? true : m_size >= m_n / 2;
	}

	/* MODIFIERS */

	/**
	 * @brief Generates next tree.
	 *
	 * Modifies the internal state so that the next tree can be retrieved using
	 * method @ref get_tree.
	 * @pre The generator must have been initialized.
	 */
	void next() noexcept
	{
		++m_size;
	}

	/// Sets the generator to its initial state.
	void reset() noexcept
	{
		m_size = 0;
	}

	[[nodiscard]] graphs::free_tree yield_tree() noexcept
	{
		const auto t = get_tree();
		next();
		return t;
	}

protected:

	/**
	 * @brief Constructs the current tree.
	 * @returns The tree generated with method @ref next().
	 * @pre The generator must have been initialized.
	 * @pre Method @ref next must have been called at least once.
	 */
	[[nodiscard]] graphs::free_tree _get_tree() noexcept;

private:

	/// The size of the left partition.
	uint64_t m_size;
};

} // namespace generate
} // namespace lal
