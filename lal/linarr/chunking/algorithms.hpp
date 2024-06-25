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

namespace lal {
namespace linarr {

// **DEVELOPER NOTE**
// This enumeration's documentation has to be updated manually in the
// algorithms_chunking.py python file.

/**
 * @brief The different algorithms for chunking a syntactic dependency tree.
 * 
 * Chunking is the art of grouping nodes (a.k.a. words) of a syntactic dependency
 * tree in such a way that the resulting groups share common properties. This
 * enumeration lists all the chunking algorithms implemented in this library.
 *
 * Here we use 'chunking' as an umbrella term for all the algorithms that
 * group nodes in units in a systematic way. Some researchers may not use this
 * term.
 */
enum class algorithms_chunking {
	/**
	 * @brief Chunking algorithm by Anderson.
	 *
	 * For further details see \cite Anderson2021a and \cite Anderson2019a.
	 */
	Anderson,
	/**
	 * @brief Chunking algorithm by Mačutek.
	 *
	 * Mačutek et al. termed chunks as Linear Dependency Sequences (LDSs).
	 * For further details see \cite Macutek2021a.
	 *
	 * Note: the implementation of Mačutek's algorithm in LAL does not take
	 * clauses into account when computing an LDS.
	 */
	Macutek
};

} // -- namespace linarr
} // -- namespace lal
