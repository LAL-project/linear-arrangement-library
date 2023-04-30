/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/linarr/chunk.hpp>
#include <lal/linarr/algorithms_chunking.hpp>
#include <lal/graphs/rooted_tree.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Chunks a syntactic dependency tree according to the algorithm passed
 * as parameter.
 * 
 * This function assumes the identity arrangement to chunk this tree.
 * @param rt Input syntactic dependency tree.
 * @param algo Algorithm of choice.
 * @returns A syntactic dependency tree result of the chosen chunking algorithm.
 */
graphs::rooted_tree chunk_syntactic_dependency_tree(
	const graphs::rooted_tree& rt,
	const algorithms_chunking& algo
) noexcept;

/**
 * @brief Chunks a syntactic dependency tree according to the algorithm passed
 * as parameter.
 * @param rt Input syntactic dependency tree.
 * @param arr Non-empty input linear arrangement.
 * @param algo Algorithm of choice.
 * @returns A syntactic dependency tree result of the chosen chunking algorithm.
 */
graphs::rooted_tree chunk_syntactic_dependency_tree(
	const graphs::rooted_tree& rt,
	const linear_arrangement& arr,
	const algorithms_chunking& algo
) noexcept;

/**
 * @brief Chunks a syntactic dependency tree according to the algorithm passed
 * as parameter.
 *
 * This function assumes the identity arrangement to chunk this tree.
 * @param rt Input syntactic dependency tree.
 * @param algo Algorithm of choice.
 * @returns The sequence of chunks as a simple collection of nodes plus parent
 * node for each chunk.
 */
chunk_sequence chunk_syntactic_dependency_tree_as_sequence(
	const graphs::rooted_tree& rt,
	const algorithms_chunking& algo
) noexcept;

/**
 * @brief Chunks a syntactic dependency tree according to the algorithm passed
 * as parameter.
 * @param rt Input syntactic dependency tree.
 * @param arr Non-empty input linear arrangement.
 * @param algo Algorithm of choice.
 * @returns The sequence of chunks as a simple collection of nodes plus parent
 * node for each chunk.
 */
chunk_sequence chunk_syntactic_dependency_tree_as_sequence(
	const graphs::rooted_tree& rt,
	const linear_arrangement& arr,
	const algorithms_chunking& algo
) noexcept;

/**
 * @brief Constructs a rooted tree from the given chunk sequence.
 * @param seq Chunk sequence
 * @returns A rooted tree.
 */
graphs::rooted_tree make_tree_from_chunk_sequence(const chunk_sequence& seq) noexcept;

} // -- namespace linarr
} // -- namespace lal
