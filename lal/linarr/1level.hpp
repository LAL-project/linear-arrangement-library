/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 *  Linear Arrangement Library is free software: you can redistribute it
 *  and/or modify it under the terms of the GNU Affero General Public License
 *  as published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  Linear Arrangement Library is distributed in the hope that it will be
 *  useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Contact:
 *
 *      Lluís Alemany Puig (lalemany@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://cqllab.upc.edu/people/rferrericancho/
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
#include <lal/graphs/graph.hpp>
#include <lal/utilities/aggregations.hpp>

namespace lal {
namespace linarr {

/* 1-LEVEL METRICS */

// **DEVELOPER NOTE**
// This function's documentation has to be updated manually in the python
// interface file '.i' 'python-interface/submodules/linarr.i'
/**
 * @brief 1-level Mean Dependency Distance \f$MDD\f$ over an ensemble
 * of graphs.
 *
 * Given a list of graphs \f$L\f$ and a list of linear arrangements for each of
 * them, \f$P\f$, computes the 1-level Mean Dependency Distance as the quotient
 * of \f$D\f$, the sum of all the edge lengths of each graph, and of \f$M\f$ the
 * sum of the number of edges of all the graphs.
 *
 * Formally, given a list of graphs \f$L = \{L_i\}_{i=1}^k\f$ and a list of
 * linear arrangements \f$\Pi = \{\pi_i\}_{i=1}^k\f$, computes \f$D/M\f$, where
 * - \f$D = \sum_{i=1}^k D(L_i, \pi_i)\f$ is the sum of edge lengths of all
 * graphs.
 * - \f$M = \sum_{i=1}^k |E(L_i)|\f$ is the sum of the number of edges of all
 * graphs.
 *
 * @param L List of input graphs.
 * @param P List of linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for every graph.
 * @tparam graph_t A graph type. A class that derives from @ref lal::graphs::graph.
 * @returns Jing's and Liu's 1-level \f$MDD\f$ for an ensemble of graphs as an
 * exact rational value.
 */
template<class graph_t>
numeric::rational mean_dependency_distance_1level_rational
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
	typedef std::pair<uint64_t, uint64_t> DD_m;

	if (P.size() == 0) {

#define IDE linear_arrangement::identity(G.get_num_nodes())
		return utilities::one_level_aggregation<ratio, true>
		(
			L.begin(), L.end(), nullptr, nullptr,
			// make values Q,R
			[](const graph_t& G) { return DD_m{sum_edge_lengths(G, IDE), G.get_num_edges()}; },
			// accumulate Q
			[](uint64_t& total, uint64_t new_value) { total += new_value; },
			// accumulate R
			[](uint64_t& total, uint64_t new_value) { total += new_value; },
			// average accumulated Q
			[](uint64_t DDs, std::size_t) { return DDs; },
			// average accumulated R
			[](uint64_t num_edges, std::size_t) { return num_edges; },
			// average accumulated Q,R
			[](uint64_t DDs, uint64_t sum_num_edges) { return ratio(DDs, sum_num_edges); }
		);
#undef IDE

	}
	else {
		return utilities::one_level_aggregation<ratio, false>
		(
			L.begin(), L.end(), P.begin(), P.end(),
			// make values Q,R
			[](const graph_t& G, const ARR& arr) { return DD_m(sum_edge_lengths(G, arr), G.get_num_edges()); },
			// accumulate Q
			[](uint64_t& total, uint64_t new_value) { total += new_value; },
			// accumulate R
			[](uint64_t& total, uint64_t new_value) { total += new_value; },
			// average accumulated Q
			[](uint64_t DDs, std::size_t) { return DDs; },
			// average accumulated R
			[](uint64_t num_edges, std::size_t) { return num_edges; },
			// average Q,R
			[](uint64_t DDs, uint64_t sum_num_edges) { return ratio(DDs, sum_num_edges); }
		);
	}
}

// **DEVELOPER NOTE**
// This function's documentation has to be updated manually in the python
// interface file '.i' 'python-interface/submodules/linarr.i'
/**
 * @brief 1-level Mean Dependency Distance \f$MDD\f$ over an ensemble of graphs.
 *
 * See @ref lal::linarr::mean_dependency_distance_1level_rational for further
 * details.
 * @param L List of input graphs.
 * @param P List of linear arrangements of the nodes \f$Pi = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for every graph.
 * @tparam graph_t A graph type. A class that derives from @ref lal::graphs::graph.
 * @returns Jing's and Liu's 1-level \f$MDD\f$ for an ensemble of graphs as a
 * floating point value.
 */
template<class graph_t>
double mean_dependency_distance_1level
(const std::vector<graph_t>& L, const std::vector<linear_arrangement>& P = {})
noexcept
{
	static_assert(std::is_base_of_v<graphs::graph, graph_t>);
	return mean_dependency_distance_1level_rational(L, P).to_double();
}

} // -- namespace linarr
} // -- namespace lal
