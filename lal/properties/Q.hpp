/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. To see the full code
 *  visit the webpage:
 *      https://github.com/lluisalemanypuig/linear-arrangement-library.git
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
#include <vector>

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/definitions.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>

namespace lal {
namespace properties {

/**
 * @brief Compute the size of \f$Q(G)\f$.
 *
 * The set \f$Q(G)\f$ of a graph \f$G\f$ is the set of pairs of independent edges.
 * Two edges are said to be independent if they do not share vertices. Therefore,
 * this function returns the amount of independent edges of this undirected graph.
 * @param g Input graph.
 * @returns The size of \f$Q(G)\f$ as an integer of arbitrary precision.
 */
numeric::integer size_Q_integer(const graphs::undirected_graph& g);
/**
 * @brief Compute the size of \f$Q(G)\f$.
 *
 * See @ref lal::properties::size_Q_integer for details.
 * @param g Input graph.
 * @returns The size of \f$Q(G)\f$ as a 64-bit integer.
 */
uint64_t size_Q(const graphs::undirected_graph& g);

/**
 * @brief Compute the size of \f$Q(G)\f$.
 *
 * The set \f$Q(G)\f$ of a graph \f$G\f$ is the set of pairs of independent edges.
 * Two edges are said to be independent if they do not share vertices. Therefore,
 * this function returns the amount of independent edges of this directed graph.
 * @param g Input graph.
 * @returns The size of \f$Q(G)\f$ as an integer of arbitrary precision.
 */
numeric::integer size_Q_integer(const graphs::directed_graph& g);
/**
 * @brief Compute the size of \f$Q(G)\f$.
 *
 * See @ref lal::properties::size_Q_integer for details.
 * @param g Input graph.
 * @returns The size of \f$Q(G)\f$ as a 64-bit integer.
 */
uint64_t size_Q(const graphs::directed_graph& g);

} // -- namespace properties
} // -- namespace lal
