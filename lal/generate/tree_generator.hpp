/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Base class for tree generators
 *
 * This class is simply a helper for postprocessing a generated tree. The
 * postprocessing actions applied to the tree depends on whether said tree is a
 * rooted tree or a free tree.
 *
 * The list of methods that govern postprocessing actions are:
 * - @ref set_normalize_tree (rooted and free trees)
 * - @ref set_calculate_size_subtrees (rooted trees)
 * - @ref set_calculate_tree_type (rooted and free trees)
 *
 * Users can deactivate all postprocessing actions with
 * @ref deactivate_all_postprocessing_actions and activate all of them with
 * @ref activate_all_postprocessing_actions.
 *
 * There are two types of classes inheriting from this base class. The
 * @e exhaustive classes, and the @e random classes. The former provide an
 * exhaustive enumeration of a given type of trees
 * (\f$\{\text{labeled}/\text{unlabeled}\} \times \{\text{rooted}/\text{free}\}\f$);
 * the latter generate a given type of tree (the aforementioned four combinations),
 * uniformly at random.
 *
 * The names of the classes inheriting from this follow one very simple pattern.
 * This is explained in the documentation of the @ref lal::generate namespace
 * (or, if you are a Python user, @e lal.generate module).
 *
 * The exhaustive classes have three different usages:
 * @code
 *		lal::generate::all_2_3_trees Gen(10);
 *		while (not Gen.end()) {
 *			const auto T = Gen.get_tree();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * @code
 *		for (lal::generate::all_2_3_trees Gen(10); not Gen.end(); Gen.next()) {
 *			const auto T = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 * @code
 *		lal::generate::all_2_3_trees Gen(10);
 *		while (not Gen.end()) {
 *			const auto T = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 *
 * Random classes are a bit simpler to use, with, basically, two different usages:
 * @code
 *		lal::generate::rand_2_3_trees Gen(10);
 *		for (int i = 0; i < N; ++i) {
 *			const auto T = Gen.get_tree();
 *			// ...
 *		}
 * @endcode
 * @code
 *		lal::generate::rand_2_3_trees Gen(10);
 *		for (int i = 0; i < N; ++i) {
 *			const auto T = Gen.yield_tree();
 *			// ...
 *		}
 * @endcode
 *
 * @tparam tree_t Type of tree.
 */
template <
	class tree_t,
	std::enable_if_t< std::is_base_of_v<graphs::tree, tree_t>, bool > = true
>
class _tree_generator {
private:
	/// Helpful Boolean value to compact 'if constexpr' expressions.
	static constexpr bool is_free = std::is_base_of_v<graphs::free_tree, tree_t>;

public:
	using generated_tree_t = tree_t;

public:
	/* CONSTRUCTORS */

	/// Default constructor
	_tree_generator() noexcept = default;

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes
	 */
	_tree_generator(const uint64_t n) noexcept : m_n(n) { }
	/**
	 * @brief Default copy constructor.
	 * @param Gen Generator of the same type.
	 */
	_tree_generator(const _tree_generator& Gen) noexcept = default;

	/**
	 * @brief Default move constructor.
	 * @param Gen Generator of the same type.
	 */
	_tree_generator(_tree_generator&& Gen) noexcept = default;

	/// Default destructor.
	virtual ~_tree_generator() noexcept = default;

	/* INITIALIZE */

	/// Initializes the tree generator.
	void init(const uint64_t n) noexcept {
		m_n = n;
		activate_all_postprocessing_actions();
	}

	/// Clears the memory used by the generator.
	void clear() noexcept { }

	/* OPERATORS */

	/**
	 * @brief Default copy assignment operator.
	 * @param Gen Generator of the same type.
	 */
	_tree_generator& operator= (const _tree_generator& Gen) = default;
	/**
	 * @brief Default move assignment operator.
	 * @param Gen Generator of the same type.
	 */
	_tree_generator& operator= (_tree_generator&& Gen) = default;

	/* GETTERS */

	/**
	 * @brief Retrieve the generated tree.
	 *
	 * This function first calls @ref __get_tree and then modifies the
	 * generated tree according to the values:
	 * - @ref set_normalize_tree
	 * - @ref set_calculate_size_subtrees
	 * - @ref set_calculate_tree_type
	 *
	 * @returns A free/rooted tree depending on the type of the class inheriting
	 * from this. The type of generation of tree differs from one type of class
	 * to another.
	 */
	[[nodiscard]] tree_t get_tree() noexcept
	{
		tree_t t = __get_tree();

		// free and rooted trees
		if (m_normalize_tree) {
			t.normalize();
		}
		if (m_calculate_tree_type) {
			t.calculate_tree_type();
		}

		// only free trees
		if constexpr (is_free) {

		}

		// only rooted trees
		if constexpr (not is_free) {
			if (m_calculate_size_subtrees) {
				t.calculate_size_subtrees();
			}
		}
		return t;
	}

	/* MODIFIERS */

	/**
	 * @brief Yields a tree, advancing the generator if necessary.
	 *
	 * In case the class that inherits from this one is @e exhaustive then this
	 * function also moves the generator forward with its appropriate method. If
	 * the class is @e random, then it just calls @ref get_tree().
	 * @returns A free/rooted tree depending on the type of the class inheriting
	 * from this. The type of generation of tree differs from one type of class
	 * to another.
	 * @post The generator advances to the next tree.
	 */
	[[nodiscard]] virtual tree_t yield_tree() noexcept = 0;

	/**
	 * @brief Activates all postprocessing actions.
	 *
	 * The full list of postprocessing actions can be found in the documentation
	 * of this class.
	 */
	void activate_all_postprocessing_actions() noexcept {
		set_normalize_tree(true);
		set_calculate_size_subtrees(true);
		set_calculate_tree_type(true);
	}

	/**
	 * @brief Deactivates all postprocessing actions.
	 *
	 * The full list of postprocessing actions can be found in the documentation
	 * of this class.
	 */
	void deactivate_all_postprocessing_actions() noexcept {
		set_normalize_tree(false);
		set_calculate_size_subtrees(false);
		set_calculate_tree_type(false);
	}

	/* SETTERS */

	/**
	 * @brief Should trees be normalized?
	 * @param v Boolean value.
	 */
	void set_normalize_tree(const bool v) noexcept {
		m_normalize_tree = v;
	}

	/**
	 * @brief Should the size of the subtrees be calculated?
	 * @param v Boolean value.
	 */
	void set_calculate_size_subtrees(const bool v) noexcept {
		m_calculate_size_subtrees = v;
	}

	/**
	 * @brief Should the tree be classified into types?
	 *
	 * See @ref lal::graphs::tree_type for details on the classification.
	 * @param v Boolean value.
	 */
	void set_calculate_tree_type(const bool v) noexcept {
		m_calculate_tree_type = v;
	}

protected:
	/**
	 * @brief Retrieve the generated tree.
	 *
	 * See the documentation of each child class to see the correct usage
	 * of this method.
	 */
	[[nodiscard]] virtual tree_t __get_tree() noexcept = 0;

protected:
	/// Number of vertices
	uint64_t m_n = 0;

	/// Normalize the generated tree.
	bool m_normalize_tree = true;
	/// Calculate the size of the subtrees of the generated rooted tree.
	bool m_calculate_size_subtrees = true;
	/// Calculate the type of tree of the generated tree.
	bool m_calculate_tree_type = true;
};

} // -- namespace generate
} // -- namespace lal
