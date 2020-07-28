/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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

// lal includes
#include <lal/numeric/rational.hpp>
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/dgraph.hpp>
#include <lal/graphs/ftree.hpp>

namespace lal {
namespace properties {

/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of in-degree about zero,
 * \f$\langle k^p \rangle\f$, of a graph using:
 *
 * \f$\langle k^p \rangle = \frac{1}{n} \sum_{i=1}^n k^p_i \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return When \f$p=2\f$ returns the second moment, when \f$p=3\f$ returns the third moment, ...
 */
numeric::rational mmt_degree_rational(const graphs::graph& g, uint32_t p);
/**
 * @brief Computes the \f$p\f$-th moment of degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref mmt_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The return value is a floating point value.
 */
double mmt_degree(const graphs::graph& g, uint32_t p);

/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of in-degree about zero,
 * \f$\langle k_{in}^p \rangle\f$, of a directed graph using:
 *
 * \f$\langle k_{in}^p \rangle = \frac{1}{n} \sum_{i=1}^n k_{in, i}^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return When \f$p=2\f$ returns the second moment, when \f$p=3\f$ returns the third moment, ...
 */
numeric::rational mmt_in_degree_rational(const graphs::dgraph& g, uint32_t p);
/**
 * @brief Computes the \f$p\f$-th moment of in-degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref mmt_in_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The return value is a floating point value.
 */
double mmt_in_degree(const graphs::dgraph& g, uint32_t p);

/**
 * @brief Computes the \f$p\f$-th moment of out-degree about zero of a directed
 * graph as an exact rational value.
 *
 * Computes the \f$p\f$-th moment of out-degree about zero,
 * \f$\langle k_{out}^p \rangle\f$, of a directed graph using:
 *
 * \f$\langle k_{out}^p \rangle = \frac{1}{n} \sum_{i=1}^n k_{out, i}^p \f$.
 *
 * where \f$n\f$ denotes the number of nodes of the graph.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return When \f$p=2\f$ returns the second moment, when \f$p=3\f$ returns the third moment, ...
 */
numeric::rational mmt_out_degree_rational(const graphs::dgraph& g, uint32_t p);
/**
 * @brief Computes the \f$p\f$-th moment of out-degree about zero of a directed
 * graph as an exact rational value.
 *
 * See @ref mmt_out_degree_rational for details.
 * @param g Input graph.
 * @param p Moment of degree.
 * @return The return value is a floating point value.
 */
double mmt_out_degree(const graphs::dgraph& g, uint32_t p);

/**
 * @brief Computes the hubiness coefficient as an exact rational number.
 *
 * The hubiness coefficient is defined as.
 *
 * \f$ h = \frac{ \langle k^2 \rangle - \langle k^2 \rangle_{linear} }
 *				{ \langle k^2 \rangle_{star} - \langle k^2 \rangle_{linear} }
 * \f$,
 *
 * where \f$\langle k^2 \rangle_{star}\f$ and \f$\langle k^2 \rangle_{linear}\f$
 * are the second moment of degree about 0 (see @ref mmt_degree_rational)
 * of a star and linear tree respectively.
 *
 * See \cite Ferrer2018a for details.
 * @param g Input graph.
 * @pre \f$n > 3\f$.
 * @return Returns the hubiness coefficient.
 */
numeric::rational hubiness_rational(const graphs::ftree& g);

/**
 * @brief Computes the hubiness coefficient as an exact rational number.
 *
 * See @ref hubiness_rational for details.
 * @param g Input graph.
 * @return The return value is a floating point value.
 */
double hubiness(const graphs::ftree& g);

} // -- namespace properties
} // -- namespace lal
