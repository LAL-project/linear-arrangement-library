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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graph.hpp>

namespace lal {
namespace properties {

/**
 * @brief Computes the second moment of degree of a graph.
 *
 * Computes the second moment of degree about zero, \f$\langle k^2 \rangle\f$,
 * of a graph using rational values:
 *
 * \f$\langle k^2 \rangle = \frac{1}{n} \sum_{i=1}^n k_i^2 \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param[in] g Input graph.
 * @return Returns the exact second moment of degree about zero.
 */
numeric::rational second_mmt_degree_rational(const graph& g);
/**
 * @brief Computes the second moment of degree of a graph.
 *
 * Computes the second moment of degree about zero, \f$\langle k^2 \rangle\f$,
 * of a graph using floating point values of double precision:
 *
 * \f$\langle k^2 \rangle = \frac{1}{n} \sum_{i=1}^n k_i^2 \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param[in] g Input graph.
 * @return Returns the exact second moment of degree about zero.
 */
double second_mmt_degree(const graph& g);

/**
 * @brief Computes the third moment of degree of a graph.
 *
 * Computes the third moment of degree about zero, \f$\langle k^3 \rangle\f$,
 * of a graph using rational values:
 *
 * \f$\langle k^3 \rangle = \frac{1}{n} \sum_{i=1}^n k_i^3 \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param[in] g Input graph.
 * @return Returns the exact third moment of degree about zero.
 */
numeric::rational third_mmt_degree_rational(const graph& g);
/**
 * @brief Computes the third moment of degree of a graph.
 *
 * Computes the third moment of degree about zero, \f$\langle k^3 \rangle\f$,
 * of a graph using floating point values of double precision:
 *
 * \f$\langle k^3 \rangle = \frac{1}{n} \sum_{i=1}^n k_i^3 \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param[in] g Input graph.
 * @return Returns the exact third moment of degree about zero.
 */
double third_mmt_degree(const graph& g);

} // -- namespace properties
} // -- namespace lal
