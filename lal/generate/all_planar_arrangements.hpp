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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
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
#include <vector>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/detail/array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of planar arrangements of a free tree.
 *
 * The arrangements generated do not take into account the symmetrical
 * arrangements produced by swapping leaves of the tree connected to the
 * same vertex. That is, the arrangements produced can be seen as arrangements
 * of labelled trees. Therefore, this class will generate \f$n!\f$ arrangements
 * for a star tree of \f$n\f$ vertices.
 *
 * In order to use this class, you must first provide the tree object in the
 * constructor. Arrangements are generated internally, i.e., they are encoded in
 * the internal state of the generator. Said state is updated using method
 * @ref next(), which updates it to encode the next arrangement in the generation.
 * In order to retrieve an arrangement, use method @ref get_arrangement(). Upon
 * initialisation, the generator encodes the first arrangement, which has to be
 * retrieved using method @ref get_arrangement().
 *
 * This class implements the algorithm in \cite Alemany2022c.
 *
 * A possible usage of this class is the following:
 * @code
 *		lal::generate::all_planar_arrangements Gen(t); // t is a free tree
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (lal::generate::all_planar_arrangements Gen(t); not Gen.end(); Gen.next()) {
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * This class also has method @ref yield_arrangement() which is aimed at
 * simplifying this class' usage:
 * @code
 *		lal::generate::all_planar_arrangements Gen(t); // t is a free tree
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class all_planar_arrangements {
public:
	/**
	 * @brief Constructor with constant reference to a free tree.
	 * @param T Input free tree
	 * @pre The object @e T is a valid tree (see @ref graphs::free_tree::is_tree).
	 */
	all_planar_arrangements(const graphs::free_tree& T) noexcept;
	/**
	 * @brief Constructor with rooted tree.
	 *
	 * This constructor transforms the input rooted tree into a free tree.
	 * @param T Input rooted tree
	 * @pre The object @e T is a valid tree (see @ref graphs::rooted_tree::is_tree).
	 */
	all_planar_arrangements(const graphs::rooted_tree& T) noexcept;

	/**
	 * @brief Default copy constructor.
	 * @param Gen Exhaustive planar arrangement generator.
	 */
	all_planar_arrangements(const all_planar_arrangements& Gen) = default;
	/**
	 * @brief Default move constructor.
	 * @param Gen Exhaustive planar arrangement generator.
	 */
	all_planar_arrangements(all_planar_arrangements&& Gen) = default;

	/// Default destructor.
	~all_planar_arrangements() noexcept = default;

	/* GETTERS */

	/**
	 * @brief Returns whether there are more arrangements to generate.
	 * @returns True if there are still more arrangements to generate.
	 * Returns false if all arrangements have been generated.
	 */
	bool end() const noexcept { return m_reached_end; }

	/**
	 * @brief Constructs the current arrangement.
	 * @return Returns the arrangement generated with method @ref next().
	 * @pre Method @ref next must have been called at least once.
	 */
	linear_arrangement get_arrangement() const noexcept;

	/* MODIFIERS */

	/**
	 * @brief Generates the next arrangement.
	 *
	 * Modifies the internal state so that the next arrangement
	 * can be retrieved using method @ref get_arrangement.
	 */
	void next() noexcept;

	/// Sets the generator to its initial state.
	void reset() noexcept;

	/**
	 * @brief Constructs the current arrangement.
	 * @return The current arrangement.
	 * @post This generator is moved to the next arrangement.
	 */
	linear_arrangement yield_arrangement() noexcept {
		const auto arr = get_arrangement();
		next();
		return arr;
	}

private:
	/**
	 * @brief A copy of a free tree.
	 *
	 * Used only when this class is constructed with a rooted tree.
	 */
	graphs::free_tree m_T_copy;
	/// Constant reference to free tree.
	const graphs::free_tree& m_T;

	/// Vertex at which we root the tree.
	node m_root;
	/// The interval of every node of the tree
	detail::array<std::vector<node>> m_intervals;

	/// Array for the bit sort algorithm
	detail::array<char> m_memory_bit_sort;

	/// Has the end of the generation been reached?
	bool m_reached_end = false;

private:
	/// Initiales the interval of every node of the tree.
	void initialise_intervals_tree() noexcept;
	/// Initialise the interval of node @e v, whose parent vertex is @e parent.
	void initialise_interval_node(node v, node parent) noexcept;
};

} // -- namespace generate
} // -- namespace lal
