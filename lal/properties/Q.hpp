/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019
 *
 *  This file is part of Linear Arrangement Library.
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
 *          Research Gate: https://www.researchgate.net/profile/Lluis_Alemany-Puig
 *
 *      Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *          LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *          CQL (Complexity and Quantitative Linguistics Lab)
 *          Office S124, Omega building
 *          Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *          Webpage: https://www.cs.upc.edu/~rferrericancho/
 *          Research Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/definitions.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/dgraph.hpp>

namespace lal {
namespace properties {

/**
 * @brief Compute the size of \f$Q(G)\f$.
 * @param g Input graph.
 * @return Returns the size of \f$Q(G)\f$ as an integer of arbitrary precision.
 */
numeric::integer size_Q_integer(const graphs::ugraph& g);
/**
 * @brief Compute the size of \f$Q(G)\f$.
 *
 * See @ref size_Q_integer for details.
 * @param g Input graph.
 * @return The return value is a simple 64-bit unsigned integer.
 */
uint32_t size_Q(const graphs::ugraph& g);

/**
 * @brief Compute the size of \f$Q(G)\f$.
 * @param g Input graph.
 * @return Returns the size of \f$Q(G)\f$ as an integer of arbitrary precision.
 */
numeric::integer size_Q_integer(const graphs::dgraph& g);
/**
 * @brief Compute the size of \f$Q(G)\f$.
 *
 * See @ref size_Q_integer for details.
 * @param g Input graph.
 * @return The return value is a simple 64-bit unsigned integer.
 */
uint32_t size_Q(const graphs::dgraph& g);

} // -- namespace properties
} // -- namespace lal
