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
#include <lal/generate/tree_gen.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive unlabelled rooted tree generator.
 *
 * Generates all the unlabelled rooted trees of a given number of nodes. The
 * algorithm implemented can be found in \cite Beyer1980a. The definition of
 * the public and private members of this class follow the notation in this
 * work.
 *
 * In order to use this class, you must first provide the size \f$n\f$ of the
 * tree (number of nodes) in the constructor or in the @ref init method of
 * the trees to be generated.
 *
 * Then, call method @ref next to modify the internal state that will allow the
 * user to construct the tree by calling method @ref get_tree.
 *
 * All the unlabelled rooted trees have been generated when method @ref has_next
 * returns false. At this point, method @ref get_tree will always construct a
 * star tree of \f$n\f$ nodes rooted at its central node.
 *
 * In order to restart the generation of these trees, call method @ref init
 * again. It is allowed, at any time, to call @ref init with the same size of
 * the trees, or with a different one.
 *
 * The correct usage of this class is
 * @code
 *		lal::generate::all_ulab_rooted_trees TreeGen(n);
 *		while (TreeGen.has_next()) {
 *			TreeGen.next();
 *			lal::graphs::rooted_tree T = TreeGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_ulab_rooted_trees : public tree_gen<graphs::rooted_tree> {
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

	/**
	 * @brief Returns whether there are more trees to generate.
	 * @returns True if there are still more trees
	 * to generate. Returns false if all trees have been
	 * generated (there are no more unique trees of this
	 * size that were not generated before).
	 */
	inline bool has_next() const noexcept { return not m_is_last; }

	/* MODIFIERS */

	/**
	 * @brief Generates the next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree.
	 * @pre The generator must have been initialised.
	 */
	void next() noexcept;

	/**
	 * @brief Sets the generator to its initial state.
	 *
	 * This method can be called anytime.
	 */
	void reset() noexcept;

protected:
	/**
	 * @brief Constructs the current tree.
	 * @returns The tree generated with method @ref next(). The tree
	 * is rooted at vertex 0.
	 * @pre The generator must have been initialised, and method
	 * @ref next must have been called at least once.
	 */
	graphs::rooted_tree __get_tree() noexcept;

	/**
	 * @brief Class initialiser method.
	 *
	 * Initialises the internal state of this generator so that method
	 * @ref next can be called safely. The number of vertices @ref m_n
	 * is initialised during construction.
	 */
	void init() noexcept;

private:
	/// Is the current tree the last tree to be generated?
	bool m_is_last = false;
	/// Is the current tree the first tree to be generated?
	bool m_is_first = false;

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
