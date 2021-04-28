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
#include <lal/definitions.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/generate/tree_generator.hpp>
#include <lal/internal/data_array.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of labelled free trees.
 *
 * Generates all the labelled free trees of a given number of nodes. The
 * algorithm implemented uses Prüfer sequences (see \cite Pruefer1918a) and
 * decodes them using the algorithm in \cite Alonso1995a.
 *
 * In order to use this class, you must first provide the size \f$n\f$ of the
 * tree (number of nodes) in the constructor or in the @ref init method of
 * the trees to be generated.
 *
 * Then, call method @ref next to modify the internal state that will allow the
 * user to construct the tree by calling method @ref get_tree.
 *
 * All the free labelled trees have been generated when method @ref has_next
 * returns false. At this point, method @ref get_tree will always construct a
 * star tree of \f$n\f$ nodes.
 *
 * In order to restart the generation of these trees, call method @ref init
 * again. It is allowed, at any time, to call @ref init with the same size of
 * the trees, or with a different one.
 *
 * An example of usage of this class is
 * @code
 *		lal::generate::all_lab_free_trees TreeGen(n);
 *		while (TreeGen.has_next()) {
 *			TreeGen.next();
 *			const lal::graphs::free_tree T = TreeGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_lab_free_trees : public tree_generator<graphs::free_tree> {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	all_lab_free_trees(uint32_t n) noexcept;
	/**
	 * @brief Copy constructor.
	 * @param Gen Exhaustive labelled free tree generator..
	 */
	all_lab_free_trees(const all_lab_free_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Exhaustive labelled free tree generator..
	 */
	all_lab_free_trees(all_lab_free_trees&& Gen) = default;
#endif
	/// Default destructor.
	~all_lab_free_trees() = default;

	/* GETTERS */

	/**
	 * @brief Returns whether there are more trees to generate.
	 * @returns True if there are still more trees to generate. Returns
	 * false if all trees have been generated (there are no more unique trees
	 * of this size that were not generated before).
	 */
	inline bool has_next() const noexcept {
		return not m_sm[(m_n <= 2 ? 0 : m_n - 3)];
	}

	/* MODIFIERS */

	/**
	 * @brief Generates next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree().
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
	 * @returns The tree generated with method @ref next().
	 * @pre The generator must have been initialised, and method
	 * @ref next must have been called at least once.
	 */
	graphs::free_tree __get_tree() noexcept;

	/**
	 * @brief Class initialiser method.
	 *
	 * Initialises the internal state of this generator so that method
	 * @ref next can be called safely. The number of vertices @ref m_n
	 * is initialised during construction.
	 */
	void init() noexcept;

private:
	/// Iterator on the sequence.
	uint32_t m_it;
	/// Left-most position with value \f$n-1\f$.
	uint32_t m_L;
	/// Prüfer sequence.
	internal::data_array<uint32_t> m_seq;
	/// If sm[i] = true iff sm[0..i-1] = true and seq[0..i] = n-2
	internal::data_array<bool> m_sm;
};

} // -- namespace generate
} // -- namespace lal
