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
 *		lal::generate::all_lab_rooted_trees Gen(n);
 *		while (not Gen.end()) {
 *			const lal::graphs::rooted_tree t = Gen.get_tree();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * Alternatively, this class can be used in a for loop:
 * @code
 *		for (lal::generate::all_lab_rooted_trees Gen(n); not Gen.end(); Gen.next()) {
 *			const lal::graphs::rooted_tree t = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 * Equivalently,
 * @code
 *		lal::generate::all_lab_rooted_trees Gen(n);
 *		while (not Gen.end()) {
 *			const lal::graphs::rooted_tree t = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_lab_rooted_trees : public _tree_generator<graphs::rooted_tree> {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	all_lab_rooted_trees() noexcept : _tree_generator<graphs::rooted_tree>() { }

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes.
	 */
	all_lab_rooted_trees(uint64_t n) noexcept {
		init(n);
	}
	/**
	 * @brief Copy constructor.
	 * @param Gen Exhaustive labelled rooted tree generator..
	 */
	all_lab_rooted_trees(const all_lab_rooted_trees& Gen) = default;

	/**
	 * @brief Move constructor.
	 * @param Gen Generator of the same type.
	 */
	all_lab_rooted_trees(all_lab_rooted_trees&& Gen) = default;

	/// Default destructor.
	~all_lab_rooted_trees() = default;

	/// Copy assignment operator.
	all_lab_rooted_trees& operator= (const all_lab_rooted_trees& g) noexcept = default;
	/// Move assignment operator.
	all_lab_rooted_trees& operator= (all_lab_rooted_trees&& g) noexcept = default;

	/* INITIALIZE */

	/**
	 * @brief Initializes the generator with a given number of vertices.
	 * @param n Number of vertices.
	 */
	void init(uint64_t n) noexcept {
		_tree_generator::init(n);
		m_gen_lab_free_tree.init(m_n);
		reset();
	}

	/**
	 * @brief Clears the memory used.
	 * @post Method @ref init must be called after every call to @ref clear.
	 */
	void clear() noexcept {
		_tree_generator::clear();
		m_cur_ftree.clear();
		m_gen_lab_free_tree.clear();
	}

	/* GETTERS */

	/// Returns true if the end of the iteration was reached.
	bool end() const noexcept { return m_reached_end; }

	/// Returns the current root.
	node get_current_root() const noexcept { return m_cur_root; }

	/* MODIFIERS */

	/**
	 * @brief Generates next tree.
	 *
	 * Modifies the internal state so that the next tree
	 * can be retrieved using method @ref get_tree().
	 * @pre The generator must have been initialized.
	 */
	void next() noexcept {
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
	void reset() noexcept {
		activate_all_postprocessing_actions();
		__reset();
		next();
	}

	/// Returns whether there are more trees to generate.
	bool has_next() const noexcept {
		return m_cur_root + 1 < m_n or not m_gen_lab_free_tree.end();
	}

	graphs::rooted_tree yield_tree() noexcept {
		const auto t = get_tree();
		next();
		return t;
	}

protected:
	/**
	 * @brief Constructs the current tree.
	 * @returns The tree generated with method @ref next().
	 * @pre The generator must have been initialized.
	 * @pre Method @ref next must have been called at least once.
	 */
	graphs::rooted_tree __get_tree() noexcept {
#if defined DEBUG
		assert(m_cur_root < m_n);
#endif
		return graphs::rooted_tree(m_cur_ftree, m_cur_root);
	}

	/// Sets the iterator to its initial state.
	void __reset() noexcept {
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
