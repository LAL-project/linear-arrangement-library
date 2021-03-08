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

// lal includes
#include <lal/graphs/rooted_tree.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace properties {

/**
 * @brief Mean Hierarchical Distance.
 *
 * The mean hierarchical distance is calculated as a mean of the different
 * hierarchical distances between each vertex and the root of the tree. The
 * hierarchical distance \f$HD_u\f$ of vertex \f$u\f$ is calculated as the
 * number of edges between the tree's root and \f$u\f$. The result of this
 * function is the mean of these distances:
 * \f$MHD = \frac{1}{n} \sum_{u\in V} HD_u\f$.
 *
 * For furhter details see \cite Jing2015a.
 * @param t Input rooted tree.
 * @returns The Mean Hierarchical Distance of a rooted tree as a rational value.
 */
numeric::rational mean_hierarchical_distance_rational(const graphs::rooted_tree& t);

/**
 * @brief Mean Hierarchical Distance.
 *
 * See @ref lal::properties::mean_hierarchical_distance_rational for details.
 * @param t Input rooted tree.
 * @returns The Mean Hierarchical Distance of a rooted tree as a floating point value.
 */
double mean_hierarchical_distance(const graphs::rooted_tree& t);

} // -- namespace properties
} // -- namespace lal
