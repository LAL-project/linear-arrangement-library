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
#include <lal/generate/all_lab_free_trees.hpp>
#include <lal/generate/tree_gen.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive labelled rooted tree generator.
 *
 * This class is based on the labelled free trees generator (see
 * @ref all_lab_free_trees).
 *
 * The correct usage of this class is
 * @code
 *		all_lab_rooted_trees TreeGen(n);
 *		while (TreeGen.has_next()) {
 *			TreeGen.next();
 *			lal::graphs::rooted_tree T = TreeGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_lab_rooted_trees : public tree_gen<graphs::rooted_tree> {
public:
	/// Constructor with number of nodes.
	all_lab_rooted_trees(uint32_t n) noexcept;
	/// Default destructor.
	~all_lab_rooted_trees() noexcept = default;

#ifndef SWIG
	/// Disallow copies.
	all_lab_rooted_trees(const all_lab_rooted_trees&) = delete;
	/// Disallow copies.
	all_lab_rooted_trees& operator= (const all_lab_rooted_trees&) = delete;
	/// Disallow moves.
	all_lab_rooted_trees(all_lab_free_trees&&) = delete;
	/// Disallow moves.
	all_lab_rooted_trees& operator= (all_lab_rooted_trees&&) = delete;
#endif

	/**
	 * @brief Returns whether there are more trees to generate.
	 * @return Returns true if there are still more trees
	 * to generate. Returns false if all trees have been
	 * generated (there are no more unique trees of this
	 * size that were not generated before).
	 */
	inline bool has_next() const noexcept {
		return
		not( m_cur_root + 1 >= m_n and (not m_gen_lab_free_tree.has_next()));
	}

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
	 * @return Returns the tree generated with method @ref next().
	 * @pre The generator must have been initialised, and method
	 * @ref next must have been called at least once.
	 */
	graphs::rooted_tree __get_tree() noexcept;

	/**
	 * @brief Initialises the generator.
	 *
	 * Initialises the internal state of this generator so that method
	 * @ref next can be called safely. The number of vertices @ref m_n
	 * is initialised during construction.
	 */
	void init() noexcept;

private:
	/// Number of nodes of the generated trees.
	const uint32_t m_n;

	/// Labelled free tree generator.
	all_lab_free_trees m_gen_lab_free_tree;
	/// Current labelled free tree.
	graphs::free_tree m_cur_ftree;
	/// Current root.
	node m_cur_root;
};

} // -- namespace generate
} // -- namespace lal
