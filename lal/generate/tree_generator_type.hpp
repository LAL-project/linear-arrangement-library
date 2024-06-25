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
#include <type_traits>

// lal includes
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>

#include <lal/generate/all_lab_free_trees.hpp>
#include <lal/generate/all_lab_rooted_trees.hpp>
#include <lal/generate/all_ulab_free_trees.hpp>
#include <lal/generate/all_ulab_rooted_trees.hpp>
#include <lal/generate/rand_lab_free_trees.hpp>
#include <lal/generate/rand_lab_rooted_trees.hpp>
#include <lal/generate/rand_ulab_free_trees.hpp>
#include <lal/generate/rand_ulab_rooted_trees.hpp>

namespace lal {
namespace generate {

/**
 * @brief Type for exhasutive enumeration of trees.
 *
 * This is used to get one of the <b>all\_</b> generator types in
 * @ref tree_generator_type.
 */
struct exhaustive_t { };
/**
 * @brief Type for random generation of trees.
 *
 * This is used to get one of the <b>rand\_</b> generator types in
 * @ref tree_generator_type.
 */
struct random_t { };

/// Shorthand to get one of @ref exhaustive_t or @ref random_t.
template <bool is_exhaustive>
struct exhaustive_random_type {
	typedef
		std::conditional_t<is_exhaustive, exhaustive_t, random_t>
		type;
};
/// Shorthand of @ref exhaustive_random_type.
template <bool is_exhaustive>
using exhaustive_random_type_t =
	typename exhaustive_random_type<is_exhaustive>::type;

static_assert(std::is_same_v<exhaustive_random_type_t<true>, exhaustive_t>);
static_assert(std::is_same_v<exhaustive_random_type_t<false>, random_t>);

/**
 * @brief Type for labelled tree generation.
 *
 * This is used to get one of the <b>\_lab\_</b> generator types in
 * @ref tree_generator_type.
 */
struct labelled_t { };

/**
 * @brief Type for unlabelled tree generation.
 *
 * This is used to get one of the <b>\_ulab\_</b> generator types in
 * @ref tree_generator_type.
 */
struct unlabelled_t { };

/// Shorthand to get one of @ref labelled_t or @ref unlabelled_t.
template <bool is_labelled>
struct labelled_unlabelled_type {
	typedef
		std::conditional_t<is_labelled, labelled_t, unlabelled_t>
		type;
};
/// Shorthand of @ref labelled_unlabelled_type.
template <bool is_labelled>
using labelled_unlabelled_type_t =
	typename labelled_unlabelled_type<is_labelled>::type;

static_assert(std::is_same_v<labelled_unlabelled_type_t<true>, labelled_t>);
static_assert(std::is_same_v<labelled_unlabelled_type_t<false>, unlabelled_t>);

/**
 * @brief Automatic tree generator type generator.
 *
 * Creates the appropriate type according to the parameters given.
 * @tparam exhaustive_random Either @ref random_t or @ref exhaustive_t.
 * @tparam labelled_unlabelled Either @ref labelled_t or @ref unlabelled_t.
 * @tparam tree_t A class inheriting either from @ref graphs::free_tree or
 * @ref graphs::rooted_tree.
 */
template <
	typename exhaustive_random,
	typename labelled_unlabelled,
	class tree_t
>
struct tree_generator_type {
	static_assert(
	std::is_same_v<exhaustive_random, exhaustive_t> ||
	std::is_same_v<exhaustive_random, random_t>
	);
	static_assert(
	std::is_same_v<labelled_unlabelled, labelled_t> ||
	std::is_same_v<labelled_unlabelled, unlabelled_t>
	);

	typedef
		std::conditional_t<
			std::is_same_v<exhaustive_random, exhaustive_t>,
			// exhaustive types
			std::conditional_t<
				std::is_same_v<labelled_unlabelled, labelled_t>,
				// labelled types
				std::conditional_t<
					std::is_base_of_v<graphs::free_tree, tree_t>,
					all_lab_free_trees,
					all_lab_rooted_trees
				>,
				// unlabelled types
				std::conditional_t<
					std::is_base_of_v<graphs::free_tree, tree_t>,
					all_ulab_free_trees,
					all_ulab_rooted_trees
				>
			>,
			// random types
			std::conditional_t<
				std::is_same_v<labelled_unlabelled, labelled_t>,
				// labelled types
				std::conditional_t<
					std::is_base_of_v<graphs::free_tree, tree_t>,
					rand_lab_free_trees,
					rand_lab_rooted_trees
				>,
				// unlabelled types
				std::conditional_t<
					std::is_base_of_v<graphs::free_tree, tree_t>,
					rand_ulab_free_trees,
					rand_ulab_rooted_trees
				>
			>
		>
	type;
};

/// Typedef of @ref tree_generator_type
template <
	typename exhaustive_random,
	typename labelled_unlabelled,
	class tree_t
>
using tree_generator_type_t =
	typename tree_generator_type<
		exhaustive_random,
		labelled_unlabelled,
		tree_t
	>::type;

// some sanity checks
static_assert(
std::is_same_v<
	tree_generator_type_t<exhaustive_t, labelled_t, graphs::free_tree>,
	all_lab_free_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t<exhaustive_t, labelled_t, graphs::rooted_tree>,
	all_lab_rooted_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t<exhaustive_t, unlabelled_t, graphs::free_tree>,
	all_ulab_free_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t< exhaustive_t, unlabelled_t, graphs::rooted_tree>,
	all_ulab_rooted_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t<random_t, labelled_t, graphs::free_tree>,
	rand_lab_free_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t<random_t, labelled_t, graphs::rooted_tree>,
	rand_lab_rooted_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t<random_t, unlabelled_t, graphs::free_tree>,
	rand_ulab_free_trees
>
);
static_assert(
std::is_same_v<
	tree_generator_type_t<random_t, unlabelled_t, graphs::rooted_tree>,
	rand_ulab_rooted_trees
>
);

} // -- namespace generate
} // -- namespace lal
