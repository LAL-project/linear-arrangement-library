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
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of labelled free trees.
 *
 * Generates all the labelled free trees of a given number of nodes. The
 * algorithm implemented uses Prüfer sequences (see \cite Pruefer1918a) and
 * decodes them in \f$O(n)\f$.
 *
 * In order to use this class, users must provide the size \f$n\f$ of the tree
 * (number of nodes) in the constructor. Trees are generated internally, i.e.,
 * trees are encoded in the internal state of the generator. Said state is
 * updated using method @ref next(), which updates it to encode the next tree in
 * the generation. In order to retrieve the tree, use method @ref get_tree().
 * Upon initialisation, the generator encodes the first tree, which has to be
 * retrieved using method @ref get_tree().
 *
 * All the labelled free trees will have been generated when method @ref end()
 * returns true. At this point, method @ref get_tree() will always construct the
 * last tree in the enumeration, whichever tree it is. In order to restart the
 * generation of these trees, call method @ref reset(). It is allowed to call
 * this method at any time.
 *
 * A possible usage of this class is the following:
 * @code
 *		lal::generate::all_lab_free_trees Gen(n);
 *		while (not Gen.end()) {
 *			const lal::graphs::free_tree t = Gen.get_tree();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (lal::generate::all_lab_free_trees Gen(n); not Gen.end(); Gen.next()) {
 *			const lal::graphs::free_tree t = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		lal::generate::all_lab_free_trees Gen(n);
 *		while (not Gen.end()) {
 *			const lal::graphs::free_tree t = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_lab_free_trees : public _tree_generator<graphs::free_tree> {
public:

	/* CONSTRUCTORS */

	/// Empty constructor.
	all_lab_free_trees() noexcept
		: _tree_generator<graphs::free_tree>()
	{ }

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	all_lab_free_trees(const uint64_t n) noexcept
	{
		init(n);
	}

	/**
	 * @brief Copy constructor.
	 * @param Gen Exhaustive labelled free tree generator.
	 */
	all_lab_free_trees(const all_lab_free_trees& Gen) noexcept = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Exhaustive labelled free tree generator.
	 */
	all_lab_free_trees(all_lab_free_trees&& Gen) noexcept = default;

	/// Default destructor.
	~all_lab_free_trees() = default;

	/// Copy assignment operator.
	all_lab_free_trees& operator= (const all_lab_free_trees& g
	) noexcept = default;
	/// Move assignment operator.
	all_lab_free_trees& operator= (all_lab_free_trees&& g) noexcept = default;

	/* INITIALIZE */

	/**
	 * @brief Initializes the generator with a given number of vertices.
	 * @param n Number of vertices.
	 */
	void init(const uint64_t n) noexcept
	{
		_tree_generator::init(n);
		m_Prufer_seq.resize(m_n <= 2 ? 1 : m_n - 2, 0);
		m_sm.resize(m_n <= 2 ? 1 : m_n - 2, 0);
		reset();
	}

	/**
	 * @brief Clears the memory used.
	 * @post Method @ref init must be called after every call to @ref clear.
	 */
	void clear() noexcept
	{
		_tree_generator::clear();
		m_Prufer_seq.clear();
		m_sm.clear();
	}

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	[[nodiscard]] bool end() const noexcept
	{
		return m_reached_end;
	}

	/* MODIFIERS */

	/**
	 * @brief Generates next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree().
	 * @pre The generator must have been initialized.
	 */
	void next() noexcept
	{
		if (not has_next() or m_reached_end) {
			m_reached_end = true;
			return;
		}

		if (m_n <= 2) {
			// there is only one tree we can make
			m_sm[0] = 1;
			return;
		}

		while (m_it > 0 and m_Prufer_seq[m_it] == m_n - 1) {
			--m_it;
		}
		++m_Prufer_seq[m_it];

		m_sm[m_it] =
			(m_Prufer_seq[m_it] == m_n - 1
				 ? (m_it == 0) or
					   (m_sm[m_it - 1] and m_Prufer_seq[m_it - 1] == m_n - 1)
				 : m_sm[m_it]);

		++m_it;
		if (m_it < m_Prufer_seq.size()) {
			std::fill(m_Prufer_seq.begin() + m_it, m_Prufer_seq.end(), 0);
		}
		m_it = m_n - 3;
	}

	/// Sets the generator to its initial state.
	void reset() noexcept
	{
		activate_all_postprocessing_actions();
		_reset();
		next();
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
	graphs::free_tree _get_tree() noexcept;

	/// Sets the generator to its initial state.
	void _reset() noexcept
	{
		m_reached_end = false;

		if (m_n <= 2) {
			m_sm[0] = 0;
			// there is only one tree we can make
			return;
		}

		m_it = 0;
		m_sm.fill(false);
		m_Prufer_seq.fill(0);
		// place 'it' at the end of the sequence
		m_it = m_n - 3;
		// make sure that the first call to next()
		// produces the sequence 0 0 ... 0
		m_Prufer_seq[m_it] = static_cast<uint64_t>(-1);
		m_L = m_n - 2;
	}

	/// Returns whether there are more trees to generate.
	[[nodiscard]] bool has_next() const noexcept
	{
		return m_sm[(m_n <= 2 ? 0 : m_n - 3)] == 0;
	}

private:

	/// Iterator on the sequence.
	uint64_t m_it;
	/// Left-most position with value \f$n-1\f$.
	uint64_t m_L;
	/// Prüfer sequence.
	detail::array<uint64_t> m_Prufer_seq;
	/// If sm[i] = true iff sm[0..i-1] = true and seq[0..i] = n-2
	detail::array<char> m_sm;
	/// Has the end of the generation been reached?
	bool m_reached_end = false;
};

} // namespace generate
} // namespace lal
