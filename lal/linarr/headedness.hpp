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
#include <lal/graphs/dgraph.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Computes the headedness of a directed graph as an exact rational number.
 *
 * Given a graph and a permutation of its vertices, computes the ratio of
 * right-branching edges over the total amount of edges. More precisely, computes
 *
 * \f$h = \frac{r}{m}\f$
 *
 * where \f$r\f$ is the number of right-branching edges and \f$m\f$ is the number
 * of edges of the graph.
 *
 * A value of 0 indicates perfect left branching, and a value of 1 indicates
 * perfect right-branching.
 * @param g Input graph.
 * @param pi Permutation of the vertices. If \f$\pi[u]=p\f$ then
 * node @e u is placed in position @e p of the arrangement.
 * @return Returns the headedness ratio as an exact rational number.
 */
numeric::rational headedness_rational(const graphs::dgraph& g, const std::vector<node>& pi);

/**
 * @brief Computes the headedness of a linearly arranged directed graph.
 *
 * See @ref headedness_rational for details.
 * @return The return value is a floating point value.
 */
double headedness(const graphs::dgraph& g, const std::vector<node>& pi);

} // -- namespace linarr
} // -- namespace lal
