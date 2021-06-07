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

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of planar arrangements of a labeled free tree.
 *
 * Generates all planar arrangements of a given free tree.
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
 * This class implements the algorithm in \cite Alemany2021c.
 *
 * A possible usage of this class is the following:
 * @code
 *		all_planar_arrangements Gen(t); // t is a free tree
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (all_planar_arrangements Gen(t); not Gen.end(); Gen.next()) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * This class also has method @ref yield_arrangement() which is aimed at
 * simplifying this class' usage:
 * @code
 *		all_planar_arrangements Gen(t); // t is a free tree
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class all_planar_arrangements {
public:
	/**
	 * @brief Constructor with constant reference to a rooted tree.
	 * @param T Input free tree
	 * @pre The object @e T is a valid tree (see @ref graphs::free_tree::is_tree).
	 */
	all_planar_arrangements(const graphs::free_tree& T) noexcept;
	/**
	 * @brief Default copy constructor.
	 * @param Gen Exhaustive planar arrangement generator.
	 */
	all_planar_arrangements(const all_planar_arrangements& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Default move constructor.
	 * @param Gen Exhaustive planar arrangement generator.
	 */
	all_planar_arrangements(all_planar_arrangements&& Gen) = default;
#endif
	/// Default destructor.
	~all_planar_arrangements() noexcept = default;

	/* GETTERS */

	/**
	 * @brief Returns whether there are more arrangements to generate.
	 * @returns True if there are still more arrangements to generate.
	 * Returns false if all arrangements have been generated.
	 */
	inline bool end() const noexcept { return m_reached_end; }

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
	inline void reset() noexcept {
		__reset();
		next();
	}

	/**
	 * @brief Constructs the current arrangement.
	 * @return Returns the arrangement generated with method @ref next().
	 * @pre Method @ref next must have been called at least once.
	 */
	inline linear_arrangement yield_arrangement() noexcept {
		const auto arr = get_arrangement();
		next();
		return arr;
	}

private:
	/// Constant reference to rooted tree.
	const graphs::free_tree& m_T;
	/// Vertex at which we root the tree.
	node m_root;
	/// The interval of every node of the tree
	std::vector<std::vector<node>> m_intervals;

	/**
	 * @brief Parent of every node with respect to the root @ref m_root.
	 *
	 * This is needed since @ref m_T is a free tree.
	 */
	internal::data_array<node> m_parent;

	/// Is there a next projective arrangement to iterate over?
	bool m_exists_next = true;
	/// Has the end of the generation been reached?
	bool m_reached_end = false;
	/// Should we use the next vertex as root?
	bool m_use_next_root = false;

private:
	/// Sets the iterator to its initial state.
	void __reset() noexcept;
	/// Initiales the interval of every node of the tree.
	void initialise_intervals_tree() noexcept;
	/// Initialise the interval of node @e v.
	void initialise_interval_node(node v) noexcept;
};

} // -- namespace generate
} // -- namespace lal