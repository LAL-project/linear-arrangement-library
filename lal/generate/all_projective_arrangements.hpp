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
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of projective arrangements of a labeled rooted tree.
 *
 * Generates all projective arrangements of a given rooted tree.
 *
 * The arrangements generated do not take into account the symmetrical
 * arrangements produced by swapping leaves of the tree connected to the
 * same parent. That is, the arrangements produced can be seen as arrangements
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
 * This class implements the algorithm outlined in \cite Gildea2007a and
 * \cite Futrell2015a.
 *
 * A possible usage of this class is the following:
 * @code
 *		all_projective_arrangements Gen(t); // t is a rooted tree
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (all_projective_arrangements Gen(t); not Gen.end(); Gen.next()) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * This class also has method @ref yield_arrangement() which is aimed at simplifying
 * this class' usage:
 * @code
 *		all_projective_arrangements Gen(t); // t is a rooted tree
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class all_projective_arrangements {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with rooted tree.
	 * @param T Rooted tree
	 * @pre The object @e T is a valid rooted tree (see
	 * @ref graphs::rooted_tree::is_rooted_tree).
	 */
	all_projective_arrangements(const graphs::rooted_tree& T) noexcept;
	/**
	 * @brief Constructor with free tree and a root vertex.
	 * @param T Free tree
	 * @param root Root vertex
	 * @pre The object @e T is a valid tree (see
	 * @ref graphs::rooted_tree::is_tree).
	 */
	all_projective_arrangements(const graphs::free_tree& T, node root) noexcept
		: all_projective_arrangements(graphs::rooted_tree(T, root))
	{ }
	/**
	 * @brief Default copy constructor.
	 * @param Gen Exhaustive projective arrangement generator.
	 */
	all_projective_arrangements(const all_projective_arrangements& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Default move constructor.
	 * @param Gen Exhaustive projective arrangement generator.
	 */
	all_projective_arrangements(all_projective_arrangements&& Gen) = default;
#endif

	~all_projective_arrangements() noexcept = default;

	/* GETTERS */

	/**
	 * @brief Returns whether there are more arrangements to generate.
	 * @returns True if there are still more arrangements to generate.
	 * Returns false if all arrangements have been generated.
	 */
	inline bool end() const noexcept { return m_reached_end; };

	/**
	 * @brief Constructs the current arrangement.
	 * @returns The arrangement generated with method @ref next().
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
	 * @brief Returns the current arrangement and advances the generator.
	 *
	 * This method calls @ref next() after retrieving a copy of the current
	 * arrangement.
	 * @returns The current arrangement.
	 */
	inline linear_arrangement yield_arrangement() noexcept {
		// yes, a copy...
		const linear_arrangement arr = get_arrangement();
		next();
		return arr;
	}

private:
	/// Constant reference to rooted tree.
	const graphs::rooted_tree& m_rT;
	/// The interval of every node of the tree
	std::vector<std::vector<node>> m_intervals;
	/// Is there a next projective arrangement to iterate over?
	bool m_exists_next = true;
	/// Has the end of the generation been reached?
	bool m_reached_end = false;

private:
	/// Resets the iterator to its initial state.
	void __reset() noexcept;
	/// Initialise the interval every node of the tree, starting at @e r.
	void initialise_intervals_tree() noexcept;
	/// Initialise the interval of node @e u.
	void initialise_interval_node(node u) noexcept;
};

} // -- namespace generate
} // -- namespace lal
