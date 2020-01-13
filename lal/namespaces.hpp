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
 * @brief Namespace for the generation of different types of graphs.
 * 
 * This namespace contains algorithms for the generation of
 * certain types of graphs, for example: all unique unlabelled free trees.
 */
namespace generate {}

/**
 * @brief Namespace for the graphs data structures.
 * 
 * This namespace contains the data structures that implement graphs. Note that,
 * although we do not support labelled graphs, the vertices of these graphs
 * carry a label, a number between 0 and \f$n-1\f$, where \f$n\f$ is the number
 * of vertices of the graph.
 */
namespace graphs {}

/**
 * @brief Input/Output functions.
 * 
 * This namespace contains the functions for input/output operations.
 * 
 * This includes reading a graph (or collection of graphs) from
 * a file. The formats supported for reading are:
 * - Edge list. See functions @ref read_edge_list(const std::string&, graphs::ugraph&, bool)
 * and @ref read_edge_list(const std::string&, graphs::dgraph&, bool).
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
 * - over the set of edges (see @ref E_iterator).
 * - over the set of pairs of independent edges \f$Q\f$ (see @ref Q_iterator).
 */
namespace iterators {}

/**
 * @brief Linear arrangements namespace.
 * 
 * This namespace contains all operations related to linear arrangements.
 * 
 * All linear arrangements are vectors of as many vertices as the
 * corresponding graph has. If \f$\pi\f$ is a linear arrangement then
 * the @e u-th position of \f$\pi\f$ contains the position of that vertex in
 * the arrangement. Formally, \f$\pi[u] = p\f$ if, and only if, vertex @e u
 * is at position @e p in the linear arrangement.
 *
 * The identity arrangement \f$\pi_I\f$ is a special case of linear arrangement
 * used in many functions. Such an arrangement is one that maps each vertex
 * into the position corresponding to their label, i.e., \f$ \pi_I(u) = u\f$.
 * 
 * This namespace contains the computation of:
 * - the number of crossings \f$C\f$ (see @ref n_crossings).
 * - the sum of the length of edges \f$D\f$.
 * - the headedness of directed graphs (see @ref headedness).
 * - the Mean Dependency Distance for single trees (see @ref MDD).
 * 
 * It also contains methods for the classification of trees into projective
 * classes (see @ref tree_structure_type for the supported classes and
 * @ref get_tree_structure_type for the function to classify the trees).
 */
namespace linarr {}

/**
 * @brief Numeric namespace.
 *
 * This namespace contains the data structures that wrap the basic structures
 * of the GMP library for integers of arbitrary precision (see @ref integer)
 * and exact rational numbers (see @ref rational).
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
 * - hubiness (see @ref hubiness),
 * - Mean Hierarchical Distance (see @ref MHD),
 * - computation of the moments of:
 * 		- degree about zero \f$ \langle k^p \rangle\f$ (see @ref mmt_degree),
 * 		- in-degree about zero \f$ \langle k_{in}^p \rangle\f$ (see @ref mmt_in_degree),
 * 		- out-degree about zero \f$ \langle k_{out}^p \rangle\f$ (see @ref mmt_out_degree),
 * - computation of the amount of pairs of independent edges \f$Q\f$ (see @ref size_Q),
 * - computation of the variance of \f$V_{rla}[D]\f$ (see @ref variance_D),
 * - computation of \f$V_{rla}[C]\f$:
 * 		- in general graphs (see @ref variance_C),
 * 		- in forests (see @ref variance_C_forest),
 * 		- in trees (see @ref variance_C_tree).
 * 
 * All calculations can be retrieved as exact rational numbers
 * (see @ref numeric::rational), but also as floating point values of
 * double precision.
 */
namespace properties {}

} // -- namespace lal
