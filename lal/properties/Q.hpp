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
 *          Resarch Gate: https://www.researchgate.net/profile/Ramon_Ferrer-i-Cancho
 *
 ********************************************************************/
 
#pragma once

// C++ includes
#include <vector>

// lal includes
#include <lal/numeric/integer.hpp>
#include <lal/definitions.hpp>
#include <lal/ugraph.hpp>

namespace lal {
namespace properties {

/**
 * @brief Enumerates the elements of \f$Q(G)\f$.
 *
 * The set \f$Q(G)\f$ is defined as the pairs of edges of \f$G\f$,
 * \f$E(G) \times E(G)\f$, that are independent, that is, that share
 * no nodes.
 * @param[in] G Input graph \f$G\f$.
 * @param[out] Q Set \f$Q(G)\f$.
 */
void enumerate_Q(const ugraph& G, std::vector<edge_pair>& Q);

/**
 * @brief Compute the size of \f$Q(G)\f$.
 * @param[in] G Input graph.
 * @return Returns the size of \f$Q(G)\f$ as a rational value.
 */
numeric::integer size_Q_integer(const ugraph& G);

/**
 * @brief Compute the size of \f$Q(G)\f$.
 * @param[in] G Input graph.
 * @returns The size of \f$Q(G)\f$.
 */
uint32_t size_Q(const ugraph& G);

} // -- namespace properties
} // -- namespace lal
