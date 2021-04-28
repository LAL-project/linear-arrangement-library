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
 * - @ref set_normalise_tree (rooted and free trees)
 * - @ref set_calculate_size_subtrees (rooted trees)
 * - @ref set_calculate_tree_type (rooted and free trees)
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
class tree_generator {
public:
	/* CONSTRUCTORS */

	/**
	 * @brief Constructor with number of nodes.
	 * @param n Number of nodes
	 */
	tree_generator(uint32_t n) noexcept : m_n(n) { }
	/**
	 * @brief Default copy constructor.
	 * @param Gen Generator of the same type.
	 */
	tree_generator(const tree_generator& Gen) = default;
#ifndef SWIG
	/**
	 * @brief Default move constructor.
	 * @param Gen Generator of the same type.
	 */
	tree_generator(tree_generator&& Gen) = default;
#endif
	/// Default destructor.
	virtual ~tree_generator() = default;

	/* OPERATORS */

#ifndef SWIG
	/**
	 * @brief Default copy assignment operator.
	 * @param Gen Generator of the same type.
	 */
	tree_generator& operator= (const tree_generator& Gen) = default;
	/**
	 * @brief Default move assignment operator.
	 * @param Gen Generator of the same type.
	 */
	tree_generator& operator= (tree_generator&& Gen) = default;
#endif
	/* GETTERS */

	/**
	 * @brief Retrieve the generated tree.
	 *
	 * This function first calls @ref __get_tree and then modifies the
	 * generated tree according to the values:
	 * - @ref set_normalise_tree
	 * - @ref set_calculate_size_subtrees
	 * - @ref set_calculate_tree_type
	 */
	inline
	std::conditional_t<is_free, graphs::free_tree, graphs::rooted_tree>
	get_tree() noexcept
	{
		auto t = __get_tree();

		// free and rooted trees
		if (m_normalise_tree) {
			t.normalise();
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

	/* SETTERS */

	/**
	 * @brief Should trees be normalised?
	 * @param v Boolean value.
	 */
	inline void set_normalise_tree(bool v) noexcept
	{ m_normalise_tree = v; }

	/**
	 * @brief Should the size of the subtrees be calculated?
	 * @param v Boolean value.
	 */
	inline void set_calculate_size_subtrees(bool v) noexcept
	{ m_calculate_size_subtrees = v; }

	/**
	 * @brief Should the tree be classified into types?
	 *
	 * See @ref lal::graphs::tree_type for details on the classification.
	 * @param v Boolean value.
	 */
	inline void set_calculate_tree_type(bool v) noexcept
	{ m_calculate_tree_type = v; }

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

	/// Normalise the generated tree.
	bool m_normalise_tree = true;
	/// Calculate the size of the subtrees of the generated rooted tree.
	bool m_calculate_size_subtrees = true;
	/// Calculate the type of tree of the generated tree.
	bool m_calculate_tree_type = true;
};

} // -- namespace generate
} // -- namespace lal
