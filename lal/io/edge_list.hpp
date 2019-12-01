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
#include <string>

// lal includes
#include <lal/graphs/ugraph.hpp>
#include <lal/graphs/dgraph.hpp>

namespace lal {
namespace io {

/**
 * @brief Reads a graph in edge list format.
 *
 * This format consists of a list of all the graph's edges. Each edge
 * is described as a pair of indices of the nodes at each end of the
 * edge.
 *
 * Nodes are usually labelled with indices starting at 0, so this value
 * is accepted. The resulting number of nodes of the graph will be the
 * maximum index in the file plus 1.
 *
 * The current contents of the graph will be cleared and replaced by
 * the contents of the file.
 * @param[in] filename Name of the file.
 * @param[out] g Graph read from the file.
 * @param[in] norm Should the graph be normalised? See @ref graphs::graph::is_normalised()
 * @return Returns 'false' if the file could not be opened. Returns 'true'
 * if the graph was read successfully.
 */
bool read_edge_list(const std::string& filename, graphs::ugraph& g, bool norm = true);
/**
 * @brief Reads a graph in edge list format.
 *
 * This format consists of a list of all the graph's edges. Each edge
 * is described as a pair of indices of the nodes at each end of the
 * edge.
 *
 * Nodes are usually labelled with indices starting at 0, so this value
 * is accepted. The resulting number of nodes of the graph will be the
 * maximum index in the file plus 1.
 *
 * The current contents of the graph will be cleared and replaced by
 * the contents of the file.
 * @param[in] filename Name of the file.
 * @param[out] g Graph read from the file.
 * @param[in] norm Should the graph be normalised? See @ref graphs::graph::is_normalised()
 * @return Returns 'false' if the file could not be opened. Returns 'true'
 * if the graph was read successfully.
 */
bool read_edge_list(const std::string& filename, graphs::dgraph& g, bool norm = true);

} // -- namespace io
} // -- namespace lal
