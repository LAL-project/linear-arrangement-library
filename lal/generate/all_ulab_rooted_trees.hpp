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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of unlabelled rooted trees.
 *
 * Generates all the unlabelled rooted trees of a given number of nodes. The
 * algorithm implemented can be found in \cite Beyer1980a. The definition of
 * the public and private members of this class follow the notation in this
 * work.
 *
 * In order to use this class, users must provide the size \f$n\f$ of the tree
 * (number of nodes) in the constructor. Trees are generated internally, i.e.,
 * trees are encoded in the internal state of the generator. Said state is
 * updated using method @ref next(), which updates it to encode the next tree in
 * the generation. In order to retrieve the tree, use method @ref get_tree().
 * Upon initialisation, the generator encodes the first tree, which has to be
 * retrieved using method @ref get_tree().
 *
 * All the unlabelled rooted trees will have been generated when method @ref end()
 * returns true. At this point, method @ref get_tree() will always construct the
 * last tree in the enumeration, whichever tree it is. In order to restart the
 * generation of these trees, call method @ref reset(). It is allowed to call
 * this method at any time.
 *
 * A possible usage of this class is the following:
 * @code
 *		all_ulab_rooted_trees Gen(n);
 *		while (not Gen.end()) {
 *			const auto t = Gen.get_tree();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (all_ulab_rooted_trees Gen(n); not Gen.end(); Gen.next()) {
 *			const auto t = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		all_ulab_rooted_trees Gen(n);
 *		while (not Gen.end()) {
 *			const auto t = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_ulab_rooted_trees : public _tree_generator<graphs::rooted_tree> {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	all_ulab_rooted_trees(uint32_t n) noexcept;
	/**
	 * @brief Copy constructor.
	 * @param Gen Exhaustive unlabelled rooted tree generator..
	 */
	all_ulab_rooted_trees(const all_ulab_rooted_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Exhaustive unlabelled rooted tree generator..
	 */
	all_ulab_rooted_trees(all_ulab_rooted_trees&& Gen) = delete;
#endif
	/// Default destructor.
	~all_ulab_rooted_trees() noexcept = default;

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	inline bool end() const noexcept { return m_reached_end; }

	/* MODIFIERS */

	/**
	 * @brief Generates the next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree.
	 * @pre The generator must have been initialised.
	 */
	void next() noexcept;

	/// Sets the generator to its initial state.
	inline void reset() noexcept {
		__reset();
		next();
	}

	inline graphs::rooted_tree yield_tree() noexcept {
		const auto t = get_tree();
		next();
		return t;
	}

protected:
	/**
	 * @brief Constructs the current tree.
	 * @returns The tree generated with method @ref next(). The tree
	 * is rooted at vertex 0.
	 * @pre The generator must have been initialised.
	 * @pre Method @ref next must have been called at least once.
	 */
	graphs::rooted_tree __get_tree() noexcept;

	/// Sets the generator to its initial state.
	void __reset() noexcept;

private:
	/// Is the current tree the last tree to be generated?
	bool m_is_last = false;
	/// Is the current tree the first tree to be generated?
	bool m_is_first = false;
	/// Has the end of the generation been reached?
	bool m_reached_end = false;

	/// Pointer as in the paper.
	uint32_t m_p = 0;
	/// Sequence SAVE
	internal::data_array<node> m_save;
	/// Sequence PREV
	internal::data_array<node> m_prev;
	/// Level sequence of the tree
	internal::data_array<node> m_L;
};

} // -- namespace generate
} // -- namespace lal
