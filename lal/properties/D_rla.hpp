/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/ugraph.hpp>

namespace lal {
namespace properties {

// D: sum of length of edges

/* -------------------------- */
/* EXPECTATION OF D: E_rla[D] */

/**
 * @brief Computes the the expectation of the first moment of D.
 *
 * This function uses the formulae derived in \cite Ferrer2019a.
 * @param g The input graph.
 * @return Returns \f$E[D]\f$ as a rational value.
 */
numeric::rational expectation_D_rational(const graphs::ugraph& g);
/**
 * @brief Computes the the expectation of the first moment of D.
 *
 * See @ref expectation_D_rational for details.
 * @param g The input graph.
 * @return The return value is a floating point value.
 */
double expectation_D(const graphs::ugraph& g);

/* ----------------------- */
/* VARIANCE OF D: V_rla[D] */

/**
 * @brief Computes the variance of the sum of the length of edges.
 *
 * Computes \f$V[D]\f$.
 *
 * This function uses the formula derived in \cite Ferrer2019a.
 * @param g Input graph.
 * @return Returns the exact value of \f$V[D]\f$ as a rational value.
 */
numeric::rational variance_D_rational(const graphs::ugraph& g);
/**
 * @brief Computes the variance of the sum of the length of edges.
 *
 * See @ref variance_D_rational for details.
 * @param g The input graph.
 * @return The return value is a floating point value.
 */
double variance_D(const graphs::ugraph& g);

} // -- namespace properties
} // -- namespace lal
