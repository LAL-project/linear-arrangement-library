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
#include <lal/graphs/ftree.hpp>

namespace lal {
namespace utils {

/*
 * @brief Converts the level sequence of a tree into a graph structure.
 *
 * Examples of level sequences:
 * -- linear tree of n nodes:
 *		0 1 2 3 4 ... (n-1) n
 * -- star tree of n nodes
 *		0 1 2 2 2 .... 2 2
 *          |------------| > (n-1) two's
 *
 * @param L The level sequence, in preorder.
 * @param n Number of nodes of the tree.
 *
 * @pre n >= 2.
 * @pre The size of L is exactly @e n + 1.
 * @pre The first value of a sequence must be a zero.
 * @pre The second value of a sequence must be a one.
 *
 * @return Returns the tree built with the sequence level @e L.
 */
graphs::ftree level_sequence_to_tree
(const std::vector<uint32_t>& L, uint32_t n);

/*
 * @brief Converts the Prüfer sequence of a labelled tree into a tree structure.
 *
 * For details on Prüfer sequences, see \cite Pruefer1918a.
 *
 * The algorithm used to decode the sequence is the one presented in
 * \cite Alonso1995a.
 * @param S The Prufer sequence sequence.
 * @param n Number of nodes of the tree.
 * @return Returns the tree built with @e L.
 */
graphs::ftree Prufer_sequence_to_tree
(const std::vector<uint32_t>& S, uint32_t n);

} // -- namespace utils
} // -- namespace lal
