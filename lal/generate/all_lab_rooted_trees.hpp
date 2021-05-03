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
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/generate/all_lab_free_trees.hpp>
#include <lal/generate/tree_generator.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of labelled rooted trees.
 *
 * This class enumerates all labelled rooted trees of a given number of vertices.
 * It is based on the labelled free trees generator (see @ref all_lab_free_trees).
 *
 * In order to use this class, users must provide the size \f$n\f$ of the tree
 * (number of nodes) in the constructor. Trees are generated internally, i.e.,
 * trees are encoded in the internal state of the generator. Said state is
 * updated using method @ref next(), which updates it to encode the next tree in
 * the generation. In order to retrieve the tree, use method @ref get_tree().
 * Upon initialisation, the generator encodes the first tree, which has to be
 * retrieved using method @ref get_tree().
 *
 * All the labelled rooted trees will have been generated when method @ref end()
 * returns true. At this point, method @ref get_tree() will always construct the
 * last tree in the enumeration, whichever tree it is. In order to restart the
 * generation of these trees, call method @ref reset(). It is allowed to call
 * this method at any time.
 *
 * A possible usage of this class is the following:
 * @code
 *		all_lab_rooted_trees Gen(n);
 *		while (not Gen.end()) {
 *			const auto t = Gen.get_tree();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, the @ref lal::generate::all_lab_rooted_trees class can be used
 * in a for loop:
 * @code
 *		for (all_lab_rooted_trees Gen(n); not Gen.end(); Gen.next()) {
 *			const auto t = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_lab_rooted_trees : public tree_generator<graphs::rooted_tree> {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	all_lab_rooted_trees(uint32_t n) noexcept
		: tree_generator<graphs::rooted_tree>(n),
		m_gen_lab_free_tree(m_n)
	{
		reset();
	}
	/**
	 * @brief Copy constructor.
	 * @param Gen Exhaustive labelled rooted tree generator..
	 */
	all_lab_rooted_trees(const all_lab_rooted_trees& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param Gen Generator of the same type.
	 */
	all_lab_rooted_trees(all_lab_rooted_trees&& Gen) = default;
#endif
	/// Default destructor.
	~all_lab_rooted_trees() = default;

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	inline bool end() const noexcept { return m_reached_end; }

	/* MODIFIERS */

	/**
	 * @brief Generates next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree().
	 * @pre The generator must have been initialised.
	 */
	inline void next() noexcept {
		if (not has_next() or m_reached_end) {
			m_reached_end = true;
			return;
		}

		// m_n > 1
		if (m_cur_root < m_n - 1) {
			++m_cur_root;
		}
		else {
			m_cur_root = 0;
			m_cur_ftree = m_gen_lab_free_tree.get_tree();
			m_gen_lab_free_tree.next();
		}
	}

	/// Sets the generator to its initial state.
	inline void reset() noexcept {
		activate_all_postprocessing_actions();
		__reset();
		next();
	}

	/// Returns whether there are more trees to generate.
	inline bool has_next() const noexcept {
		return m_cur_root + 1 < m_n or not m_gen_lab_free_tree.end();
	}

protected:
	/**
	 * @brief Constructs the current tree.
	 * @returns The tree generated with method @ref next().
	 * @pre The generator must have been initialised, and method
	 * @ref next must have been called at least once.
	 */
	inline graphs::rooted_tree __get_tree() noexcept {
#if defined DEBUG
		assert(m_cur_root < m_n);
#endif
		return graphs::rooted_tree(m_cur_ftree, m_cur_root);
	}

	/// Sets the iterator to its initial state.
	inline void __reset() noexcept {
		m_reached_end = false;
		// so that in the next call the root is set to 0
		m_cur_root = m_n - 1;
		// the labeled free tree generator now points to the first tree
		m_gen_lab_free_tree.reset();
		// deactivate all postprocessing of the free tree
		m_gen_lab_free_tree.deactivate_all_postprocessing_actions();
	}

private:
	/// Labelled free tree generator.
	all_lab_free_trees m_gen_lab_free_tree;
	/// Current labelled free tree.
	graphs::free_tree m_cur_ftree;
	/// Current root.
	node m_cur_root;
	/// Has the end of the generation been reached?
	bool m_reached_end = false;
};

} // -- namespace generate
} // -- namespace lal
