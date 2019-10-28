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
#include <lal/ugraph.hpp>

namespace lal {
namespace convert {

/**
 * @brief Converts the level sequence of a tree into a graph structure.
 * @param L The level sequence, in preorder.
 * @param n Number of nodes of the tree.
 * @return Returns the tree built with @e L.
 */
ugraph level_sequence_to_tree(const std::vector<uint32_t>& L, uint32_t n);

/**
 * @brief Converts a linear sequence of a tree to a graph structure.
 *
 * A linear sequence of a tree of @e n vertices is an array of
 * @e n + 1 integers where the values in the positions from 1 to @e n, both
 * included, describe the tree. The value '0' indicates the root, while a
 * strictly positive value indicates parent node.
 * @param L The linear sequence. It must have size @e n + 1.
 * @param n Number of nodes of the tree.
 * @return Returns the tree built with @e L.
 */
ugraph linear_sequence_to_tree(const std::vector<uint32_t>& L, uint32_t n);

/**
 * @brief Converts the Prüfer sequence of a labelled tree into a tree structure.
 *
 * For details on Prüfer sequences, see |cite Pruefer1918a.
 *
 * The algorithm used to decode the sequence is the one presented in
 * \cite Alonso1995a.
 * @param S The Prufer sequence sequence.
 * @param n Number of nodes of the tree.
 * @return Returns the tree built with @e L.
 */
ugraph Prufer_sequence_to_tree(const std::vector<uint32_t>& S, uint32_t n);

} // -- namespace convert
} // -- namespace lal
