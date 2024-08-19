/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
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
#include <lal/graphs/graph.hpp>
#include <lal/linear_arrangement.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of arrangements of any graph.
 *
 * This class generates all \f$n!\f$ arrangements of an \f$n\f$-vertex tree.
 * Unlike other generators (e.g. @ref lal::generate::all_projective_arrangements),
 * this class need not be instantiated with a tree but, rather, with a number of
 * vertices due to the simple fact that the tree structure does not matter for
 * the generation of these arrangements. However, constructing this class with
 * a tree (or any graph), is allowed for the sake of consistency.
 *
 * In order to use this class, you must first provide the number of vertices.
 * Arrangements are generated internally, i.e., arragements are encoded in the
 * internal state of the generator. Said state is updated using method @ref next(),
 * which updates it to encode the next arrangement in the generation. In order
 * to retrieve an arrangement, use method @ref get_arrangement(). Upon initialisation,
 * the generator encodes the first arrangement, which has to be retrieved using
 * method @ref get_arrangement().
 *
 * This class can also be instantiated with a graph. Neverthless, only its number
 * of vertices is actually needed.
 *
 * This class is a wrapper over the C++'s std::next_permutation algorithm.
 *
 * A possible usage of this class is the following:
 * @code
 *		lal::generate::all_arrangements Gen(g); // t is any graph (tree included)
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * When using @ref get_arrangement() in combination with @ref next(), it is important
 * to bear in mind that, if the arrangement is declared as a constant reference,
 * users should <b>not</b> call @ref next() before using the retrieved arrangement.
 * To alleviate this source of bugs, this class can also be used in a for loop:
 * @code
 *		for (lal::generate::all_arrangements Gen(9); not Gen.end(); Gen.next()) {
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * This class also has method @ref yield_arrangement() which is aimed at avoiding
 * potential source of bugs:
 * @code
 *		lal::generate::all_arrangements Gen(8);
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 * Note that the arrangement is not declared as a constant reference since
 * method @ref yield_arrangement() returns the tree as a copy.
 */
class all_arrangements {
public:
	/**
	 * @brief Constructor with graph.
	 * @param g Input graph. Only its number of vertices is used.
	 */
	all_arrangements(const lal::graphs::graph& g) noexcept
		: all_arrangements(g.get_num_nodes())
	{ }

	/**
	 * @brief Constructor with number of vertices.
	 * @param n Number of vertices of the arrangements.
	 */
	all_arrangements(const uint64_t n) noexcept
		: m_n(n),
		  m_arr(linear_arrangement(m_n))
	{
		reset();
	}

	/**
	 * @brief Returns the current linear arrangemnt.
	 *
	 * Recall that method @ref next() should <b>not</b> be called until the
	 * arrangement has been processed if such an arrangement was declared as a
	 * constant reference.
	 * @returns A permutation of the vertices (a.k.a. a linear arrangement).
	 */
	[[nodiscard]] const linear_arrangement& get_arrangement() const noexcept
	{ return m_arr; }

	/// Returns true if the end of the iteration was reached.
	[[nodiscard]] bool end() const noexcept { return m_reached_end; }

	/**
	 * @brief Generates the next arrangement.
	 *
	 * Modifies the internal state so that the next arrangement
	 * can be retrieved using method @ref get_arrangement.
	 */
	void next() noexcept;

	/// Sets the generator to its initial state.
	void reset() noexcept {
		m_reached_end = false;
		m_arr.identity();
	}

	/**
	 * @brief Constructs the current arrangement.
	 * @return The current arrangement.
	 * @post This generator is moved to the next arrangement.
	 */
	[[nodiscard]] linear_arrangement yield_arrangement() noexcept {
		// yes, a copy...
		const linear_arrangement arr = get_arrangement();
		next();
		return arr;
	}

private:
	/// Number of vertices
	const uint64_t m_n;
	/**
	 * @brief The arrangement generated by this class
	 *
	 * Actually, generated by the std::next_permutation algorithm.
	 */
	linear_arrangement m_arr;
	/// Has the end of the iteration been reached?
	bool m_reached_end = false;
};

} // -- namespace generate
} // -- namespace lal
