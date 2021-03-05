#pragma once

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Base class for tree generators
 *
 * This class is simply a helper for preprocessing a generated tree. The preprocessing
 * applied to the tree depends on whether said tree is a rooted tree or a free
 * tree.
 *
 * In the following list you will find all the attributes that govern the preprocessing.
 * Those marked with an 'r', are only for rooted trees; those marked with an 'f'
 * are only for free trees. When marked with both, the preprocessing is applied
 * to both types trees. The list of said attributes is:
 * - @ref normalise_tree (rf)
 * - @ref calculate_size_subtrees (r)
 * - @ref calculate_tree_type (rf)
 *
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
	/// Normalise the generate tree.
	bool normalise_tree = true;
	/// Calculate the size of the subtrees of the generated rooted tree.
	bool calculate_size_subtrees = true;
	/// Calculate the type of tree of the generated tree.
	bool calculate_tree_type = true;

public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes
	 */
	tree_gen(uint32_t n) noexcept : m_n(n) { }

	/**
	 * @brief Default copy constructor.
	 * @param Gen Generator of the same type.
	 */
	tree_gen(const tree_gen& Gen) noexcept = default;

	/**
	 * @brief Default move constructor.
	 * @param Gen Generator of the same type.
	 */
	tree_gen(tree_gen&& Gen) noexcept = default;

	/// Default destructor.
	virtual ~tree_gen() noexcept = default;

	/* OPERATORS */

	/**
	 * @brief Default copy assignment operator.
	 * @param Gen Generator of the same type.
	 */
	tree_gen& operator= (const tree_gen& Gen) noexcept = default;

	/**
	 * @brief Default move assignment operator.
	 * @param Gen Generator of the same type.
	 */
	tree_gen& operator= (tree_gen&& Gen) noexcept = default;

	/* GETTERS */

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
	get_tree() noexcept
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
	__get_tree()
	noexcept = 0;

	/// Number of vertices
	const uint32_t m_n;
};

} // -- namespace generate
} // -- namespace lal
