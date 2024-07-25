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

// lal includes
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>

namespace lal {
namespace properties {

/**
 * @brief Calculates the coloring of a bipartite graph.
 * @param g Input undirected graph.
 * @returns An object of type @ref lal::properties::bipartite_graph_coloring.
 * @pre The graph must be bipartite.
 */
[[nodiscard]] bipartite_graph_coloring bipartite_coloring
(const graphs::undirected_graph& g)
noexcept;

/**
 * @brief Calculates the coloring of a bipartite graph.
 *
 * This function converts the input directed graph into an undirected graph (see
 * @ref lal::graphs::directed_graph::to_undirected()).
 * @param g Input directed graph.
 * @returns An object of type @ref lal::properties::bipartite_graph_coloring.
 * @pre The underlying undirected graph must be bipartite.
 */
[[nodiscard]] bipartite_graph_coloring bipartite_coloring
(const graphs::directed_graph& g)
noexcept;

/**
 * @brief Is a graph bipartite?
 * @param g Input undirected graph.
 * @param c Coloring of the input graph.
 * @returns Whether or not the input graph is bipartite.
 */
[[nodiscard]] bool is_graph_bipartite
(const graphs::undirected_graph& g, const bipartite_graph_coloring& c)
noexcept;
/**
 * @brief Is a graph bipartite?
 * @param g Input undirected graph.
 * @returns Whether or not the input graph is bipartite.
 */
[[nodiscard]] bool is_graph_bipartite(const graphs::undirected_graph& g) noexcept;

/**
 * @brief Is a graph bipartite?
 * @param g Input directed graph.
 * @param c Coloring of the input graph.
 * @returns Whether or not the input graph is bipartite.
 */
[[nodiscard]] bool is_graph_bipartite
(const graphs::directed_graph& g, const bipartite_graph_coloring& c)
noexcept;
/**
 * @brief Is a graph bipartite?
 * @param g Input directed graph.
 * @returns Whether or not the input graph is bipartite.
 */
[[nodiscard]] bool is_graph_bipartite(const graphs::directed_graph& g) noexcept;

} // -- namespace properties
} // -- namespace lal
