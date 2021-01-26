#pragma once

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Base class for tree generators
 *
 * This class is simply a helper for postprocessing a generated tree.
 * @param T Type of tree.
 * @param is_free This tells whether the type corresponds to a free tree or not.
 */
template<
	class T,
	bool is_free = std::is_base_of_v<T, graphs::free_tree>,
	std::enable_if_t<
		std::is_base_of_v<graphs::free_tree, T> ||
		std::is_base_of_v<graphs::rooted_tree, T>,
		bool
	> = true
>
class tree_gen {
public:
	virtual ~tree_gen() { }

	/// Normalise the generate tree.
	bool normalise_tree = true;
	/// Calculate the size of the subtrees of the generated rooted tree.
	bool calculate_size_subtrees = true;
	/// Calculate the type of tree of the generated tree.
	bool calculate_tree_type = true;

	/**
	 * @brief Retrieve the generated tree.
	 *
	 * This function first calls @ref __get_tree and then modifies the
	 * generated tree according to the values:
	 * - @ref normalise_tree
	 * - @ref calculate_size_subtrees
	 * - @ref calculate_tree_type
	 */
	std::conditional_t<is_free, graphs::free_tree, graphs::rooted_tree>
	get_tree()
	{
		auto t = __get_tree();

		// free and rooted trees
		if (normalise_tree) {
			t.normalise();
		}
		if (calculate_tree_type) {
			t.calculate_tree_type();
		}

		// only free trees
		if constexpr (is_free) {

		}

		// only rooted trees
		if constexpr (not is_free) {
			if (calculate_size_subtrees) {
				t.calculate_size_subtrees();
			}
		}
		return t;
	}

protected:

	/**
	 * @brief Retrieve the generated tree.
	 *
	 * See the documentation of each child class to see the correct usage
	 * of this method.
	 */
	virtual
	std::conditional_t<is_free, graphs::free_tree, graphs::rooted_tree>
	__get_tree() = 0;
};

} // -- namespace generate
} // -- namespace lal
