/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <lal/generate/tree_gen.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive labelled free tree generator.
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
 * The correct usage of this class is
 * @code
 *		all_lab_free_trees TreeGen(n);
 *		while (TreeGen.has_next()) {
 *			TreeGen.next();
 *			lal::graphs::free_tree T = TreeGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_lab_free_trees : public tree_gen<graphs::free_tree> {
public:
	/// Constructor with number of nodes.
	all_lab_free_trees(uint32_t n) noexcept;
	/// Destructor.
	~all_lab_free_trees() noexcept;

#ifndef SWIG
	/// Disallow copies.
	all_lab_free_trees(const all_lab_free_trees&) = delete;
	/// Disallow copies.
	all_lab_free_trees& operator= (const all_lab_free_trees&) = delete;
	/// Disallow moves.
	all_lab_free_trees(all_lab_free_trees&&) = delete;
	/// Disallow moves.
	all_lab_free_trees& operator= (all_lab_free_trees&&) = delete;
#endif

	/**
	 * @brief Returns whether there are more trees to generate.
	 * @return Returns true if there are still more trees
	 * to generate. Returns false if all trees have been
	 * generated (there are no more unique trees of this
	 * size that were not generated before).
	 * @pre The generator must have been initialised.
	 */
	bool has_next() const;

	/**
	 * @brief Generates next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree().
	 * @pre The generator must have been initialised.
	 */
	void next();

	/**
	 * @brief Sets the generator to its initial state.
	 *
	 * This method can be called anytime.
	 */
	void reset();

protected:
	/**
	 * @brief Constructs the current tree.
	 * @return Returns the tree generated with method @ref next().
	 * @pre The generator must have been initialised, and method
	 * @ref next must have been called at least once.
	 */
	graphs::free_tree __get_tree();

	/**
	 * @brief Class initialiser method.
	 *
	 * Initialises the internal state of this generator so that method
	 * @ref next can be called safely.
	 *
	 * Initialising this class already allows the user to retrieve the first
	 * tree via method @ref get_tree.
	 *
	 * It is allowed to call this method two or more times, and with different
	 * values for parameter @e _n.
	 * @param _n The number of nodes of the trees to be generated.
	 */
	void init(uint32_t _n);

private:

	/**
	 * @brief Number of nodes of the tree.
	 *
	 * Size of the sequence: \f$n - 2\f$.
	 */
	uint32_t m_n;
	/// Iterator on the sequence.
	uint32_t m_it;
	/// Left-most position with value \f$n-1\f$.
	uint32_t m_L;
	/// Prüfer sequence.
	uint32_t *m_seq = nullptr;
	/// If sm[i] = true iff sm[0..i-1] = true and seq[0..i] = n-2
	char *m_sm = nullptr;
};

} // -- namespace generate
} // -- namespace lal
