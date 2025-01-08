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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif
#include <vector>

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/utilities/aggregations.hpp>

namespace lal {
namespace linarr {

/* 2-LEVEL METRICS */

// **DEVELOPER NOTE**
// This function's documentation has to be updated manually in the python
// interface file '.i' 'python-interface/submodules/linarr.i'
/**
 * @brief 2-level Mean Dependency Distance \f$MDD\f$ over an ensemble of graphs.
 *
 * Given a list of graphs \f$L\f$ and a list of linear arrangements of the nodes
 * for each of them, \f$P\f$, computes the 2-level Mean Dependency Distance, i.e.,
 * it computes the average Mean Dependency Distance of the graphs in the list.
 *
 * Formally, given a list of graphs \f$L = \{L_i\}_{i=1}^k\f$ and a list of linear
 * arrangements \f$P = \{\pi_i\}_{i=1}^k\f$, computes \f$(1/k)S_{<d>}\f$,
 * where \f$S_{<d>} = \sum_{i=1}^k MDD(L_i, \pi_i)\f$ is the sum of the mean
 * dependency distances of every graph (see
 * @ref lal::linarr::mean_dependency_distance_rational for details on the
 * definition of the Mean Dependency Distance).
 *
 * @param L List of input graphs.
 * @param P List of linear arrangements of the nodes \f$P = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for every graph.
 * @tparam graph_t A graph type. A class that derives from @ref lal::graphs::graph.
 * @returns Jing's and Liu's 2-level \f$MDD\f$ for an ensemble of graphs as an
 * exact rational value.
 */
template <class graph_t>
[[nodiscard]] numeric::rational mean_dependency_distance_2level_rational
(const std::vector<graph_t>& L, const std::vector<linear_arrangement>& P = {})
noexcept
{
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);

#if defined DEBUG
	// the number of graphs and number of linear arrangements
	// must coincide unless no arrangement was given.
	assert(P.size() == 0 or L.size() == P.size());
#endif

	typedef numeric::rational ratio;
	typedef linear_arrangement ARR;

	if (P.size() == 0) {

		return utilities::two_level_aggregation<ratio, true>
		(
			L.begin(), L.end(), nullptr, nullptr,
			// values
			[](const graph_t& G) {
				return mean_dependency_distance_rational(
					G,
					linear_arrangement::identity(G.get_num_nodes())
				);
			},
			// combine values
			[](const ratio& MDD) { return MDD; },
			// accumulate
			[](ratio& total, const ratio& MDD) { total += MDD; },
			// average the values
			[](const ratio& sum_MDD, std::size_t num_graphs) { return sum_MDD/num_graphs; }
		);

	}
	else {
		return utilities::two_level_aggregation<ratio, false>
		(
			L.begin(), L.end(), P.begin(), P.end(),
			// values
			[](const graph_t& G, const ARR& arr) { return mean_dependency_distance_rational(G, arr); },
			// combine values
			[](const ratio& MDD) { return MDD; },
			// accumulate
			[](ratio& sum_MDD, const ratio& MDD) { sum_MDD += MDD; },
			// average the values
			[](const ratio& sum_MDD, std::size_t num_graphs) { return sum_MDD/num_graphs; }
		);
	}
}

// **DEVELOPER NOTE**
// This function's documentation has to be updated manually in the python
// interface file '.i' 'python-interface/submodules/linarr.i'
/**
 * @brief 2-level Mean Dependency Distance \f$MDD\f$ over an ensemble of graphs.
 *
 * See @ref lal::linarr::mean_dependency_distance_2level_rational for details.
 * @param L List of input graphs.
 * @param P List of linear arrangements of the nodes \f$L = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for every graph.
 * @tparam graph_t A graph type. A class that derives from @ref lal::graphs::graph.
 * @returns Jing's and Liu's 2-level \f$MDD\f$ for an ensemble of graphs as a
 * floating point value.
 */
template <class graph_t>
[[nodiscard]] double mean_dependency_distance_2level
(const std::vector<graph_t>& L, const std::vector<linear_arrangement>& P = {})
noexcept
{
	return mean_dependency_distance_2level_rational(L, P).to_double();
}

} // -- namespace linarr
} // -- namespace lal
