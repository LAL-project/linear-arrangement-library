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
#include <lal/definitions.hpp>
#include <lal/numeric/rational.hpp>

namespace lal {
namespace linarr {

/* 2-LEVEL METRICS */

/**
 * @brief Computes the 2-level Mean Dependency Distance over an ensemble of graphs.
 *
 * Given a list of graphs and a linear arrangement of the nodes for each of
 * them, computes the 2-level Mean Dependency Distance, i.e., it computes the
 * average Mean Dependency Distance of the graphs in the list.
 *
 * Formally, given a list of linear arrangements \f$\Pi = \{\pi_i\}_{i=1}^k\f$
 * and a list of graphs \f$G = \{G_i\}_{i=1}^k\f$, computes \f$(1/k)S_{<d>}\f$,
 * where \f$S_{<d>} = \sum_{i=1}^k MDD(G_i, \pi_i)\f$ is the sum of the mean
 * dependency distances of every graph (see @ref MDD_rational for details on
 * the definition of the Mean Dependency Distance).
 *
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for all graphs.
 * @return Returns Jing's and Liu's 2-level \f$MDD\f$ for an ensemble of graphs.
 */
template<class G>
numeric::rational MDD_2level_rational
(const std::vector<G>& Gs, const std::vector<linearrgmnt>& pis = {});

/**
 * @brief Computes the 2-level Mean Dependency Distance over an ensemble of graphs.
 *
 * See @ref MDD_2level_rational for details.
 * @param Gs List of input graphs.
 * @param pis List of linear arrangements of the nodes \f$\Pi = \{\pi_i\}_{i=1}^k\f$.
 * When omitted, \f$\pi_I\f$ is used for all graphs.
 * @return The return value is a floating point value.
 */
template<class G>
double MDD_2level
(const std::vector<G>& Gs, const std::vector<linearrgmnt>& pis = {});

} // -- namespace linarr
} // -- namespace lal

#include <lal/linarr/2level_impl.hpp>
