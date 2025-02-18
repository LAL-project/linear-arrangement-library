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
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the headedness of a directed graph as an exact rational number.
 *
 * Given a graph and a permutation of its nodes, the headedness \f$h\f$
 * is the ratio of right-branching edges over the total amount of edges. More
 * precisely, it is
 *
 * \f$h = \frac{r}{m}\f$
 *
 * where \f$r\f$ is the number of right-branching edges and \f$m\f$ is the number
 * of edges of the graph.
 *
 * A value of 0 indicates perfect left branching, and a value of 1 indicates
 * perfect right-branching. See \cite Liu2010a for further detals.
 * @param g Input graph.
 * @param pi Permutation of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The headedness ratio as an exact rational number.
 * @pre \f$m > 0\f$.
 */
[[nodiscard]] numeric::rational head_initial_rational(
	const graphs::directed_graph& g, const linear_arrangement& pi = {}
) noexcept;

/**
 * @brief Computes the headedness of a linearly arranged directed graph.
 *
 * See @ref lal::linarr::head_initial_rational for details.
 * @param g Input graph.
 * @param pi Permutation of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The return value is a floating point value.
 * @pre \f$m > 0\f$.
 */
[[nodiscard]] double head_initial(
	const graphs::directed_graph& g, const linear_arrangement& pi = {}
) noexcept;

} // namespace linarr
} // namespace lal
