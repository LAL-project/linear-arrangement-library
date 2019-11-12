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

/* In this file are declared all namespaces of the LAL library.
 * 
 * This is done for documentation purposes only.
 */

/**
 * @brief Main namespace of the library.
 *
 * This namespace groups all namespaces of the library. Each namespace
 * classifies the classes and functions in categories.
 */
namespace lal {

/**
 * @brief Conversion functions.
 * 
 * This namespace contains functions for converting sequences
 * into graphs. Two examples of sequences are: Prufer sequences
 * (see \cite Pruefer1918a), and level sequences of a tree.
 */
namespace convert {}

/**
 * @brief Namespace for the generation of different types of graphs.
 * 
 * This namespace contains algorithms for the generation of
 * certain types of graphs, for example: all unique free,
 * unlabelled trees.
 */
namespace generate {}

/**
 * @brief Namespace for the graphs data structures.
 * 
 * This namespace contains the data structures that implement graphs.
 */
namespace graphs {}

/**
 * @brief Input/Output functions.
 * 
 * This namespace contains the functions for input/output operations.
 * 
 * This includes reading a graph (or collection of graphs) from
 * a file. The formats supported for reading are:
 * - Edge list. See function @ref read_edge_list(const std::string&, graph&, bool)
 * 
 * Other features contained in this namespace are:
 * - Processing of whole corpora of treebanks automatically (see @ref io::treebank_processor)
 * - Custom processing of whole coropora of treebanks (see @ref io::treebank_dataset)
 * - Custom processing of single treebank files (see @ref io::treebank_reader)
 */
namespace io {}

/**
 * @brief Iterators namespace.
 * 
 * This namespace contains classes useful for iterating through the graph.
 * These classes iterate
 * - over the set of edges (see @ref edge_iterator).
 * - over the set of pairs of independent edges \f$Q\f$ (see @ref Q_iterator).
 */
namespace iterators {}

/**
 * @brief Linear arrangements namespace.
 * 
 * This namespace holds all operations related to linear arrangements.
 * 
 * All linear arrangements are vectors of as many nodes as the
 * corresponding graph has. If \f$\pi\f$ is a linear arrangement then
 * the @e u-th position of \f$\pi\f$ contains the position of that node in
 * the arrangement. Formally, \f$\pi[u] = p\f$ if, and only if, node @e u
 * is at position @e p in the linear arrangement.
 * 
 * For example, it contains:
 * - Computation of the number of crossings \f$C\f$.
 * - Computation of the sum of the length of edges \f$D\f$.
 * - Computation of headedness of directed graphs.
 * - Classification of trees into classes (see @ref tree_structure_type).
 */
namespace linarr {}

/**
 * @brief Numeric namespace.
 *
 * This namespace contains the data structures that wrap the
 * basic structures of the GMP library for big integer exact
 * rational representation.
 */
namespace numeric {

	/**
	 * @brief GMP utils.
	 *
	 * This namespace contains a collection of useful algorithms
	 * for manipulating the basic data structures of the GMP library.
	 */
	namespace gmp_utils {}
}

/**
 * @brief Properties of graphs.
 * 
 * This namespace contains basic properties of graphs including, but
 * not limited to:
 * - hubiness,
 * - Mean Hierarchical Distance
 * - computation of the moments of degree about zero \f$ \langle k^p \rangle\f$,
 * - computation of the amount of pairs of independent edges \f$Q\f$,
 * - computation of the variance of \f$D\f$ and \f$C\f$ (with adhoc functions for trees in the latter case),
 * 
 * All calculations can be retrieved as exact rational numbers
 * (see @ref numeric::rational), but also as floating point values of
 * double precision.
 */
namespace properties {}

} // -- namespace lal
