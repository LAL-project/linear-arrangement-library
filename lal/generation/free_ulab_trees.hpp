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
#include <vector>

// lal includes
#include <lal/graphs/utree.hpp>
#include <lal/definitions.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive free unlabelled tree generator.
 *
 * Generates all the nonisomorphic free unlabelled trees
 * of a certain size.
 *
 * In order to use this class, the user must first provide
 * the size \f$n\f$ in the constructor or in the @ref init
 * method of the trees to be generated (number of vertices).
 *
 * Then, call method @ref next to modify the internal state
 * that will allow the user to construct the tree by calling
 * method @ref get_tree.
 *
 * All the free nonisomorphic unlabelled trees are generated when
 * method @ref has_next returns false. At this point, method
 * @ref get_tree will always construct a star tree of @e n vertices.
 *
 * In order to restart the generation of these trees, call
 * method @ref init again. It is allowed, at this point, and
 * at any time, to call @ref init with the same size of the
 * trees, or with a different one.
 *
 * The algorithm implemented can be found in the work by Wright
 * et al. (see \cite Wright1986a). The definition of the public
 * and private members of this class follow the notation in this work.
 *
 * The correct usage of this class is
 * @code
 *		free_lab_trees flt(n);
 *		while (flt.has_next()) {
 *			flt.next();
 *			ugraph e = it.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class free_ulab_trees {
	public:
		/// Default constructor.
		free_ulab_trees();
		/// Constructor with size of tree.
		free_ulab_trees(uint64_t n);
		/// Default destructor
		~free_ulab_trees();

		/**
		 * @brief Initialises the generator.
		 *
		 * Initialises the internal state of this generator
		 * so that method @ref next can be called safely.
		 *
		 * Initialising this class already allows the user
		 * to retrieve the first tree via method @ref get_tree.
		 *
		 * It is allowed to call this method two or more times,
		 * and with different values for parameter @e n.
		 * @param n The number of vertices of the trees to be
		 * generated.
		 */
		void init(uint64_t n);

		/**
		 * @brief Returns whether there are more trees to generate.
		 * @return Returns true if there are still more trees
		 * to generate. Returns false if all trees have been
		 * generated (there are no more unique trees of this
		 * size that were not generated before).
		 * @pre The generator must have been initialised.
		 */
		bool has_next() const;

		/**
		 * @brief Generates next tree.
		 *
		 * Modifies the internal state so that the next tree
		 * can be retrieved using method @ref get_tree.
		 * @pre The generator must have been initialised.
		 */
		void next();

		/**
		 * @brief Constructs the current tree.
		 * @pre The generator must have been initialised, and method
		 * @ref next must have been called at least once.
		 * @return Returns the tree generated with method @ref next().
		 */
		graphs::utree get_tree() const;

	private:
		/// Canonical level sequence of the tree.
		std::vector<uint64_t> m_L;
		/**
		 * @brief \f$W_i\f$ is the subscript of the level number in \f$L\f$
		 * corresponding to the parent of the vertex corresponding to \f$l_i\f$.
		 */
		std::vector<uint64_t> m_W;

		/// Largest integer such that \f$l_p \neq 2\f$.
		uint64_t m_p;
		/// Largest integer such that \f$q < p, \; l_q = l_p - 1\f$.
		uint64_t m_q;

		/// Maximum level number in the first principal subsequence.
		uint64_t m_h1;
		/// Maximum level number in the second principal subsequence.
		uint64_t m_h2;

		/**
		 * @brief An index to the first element of \f$L_2\f$.
		 *
		 * \f$L_2\f$ is the second principal subsequence of \f$L\f$.
		 */
		uint64_t m_c;
		/**
		 * @brief Exactly \f$m - 1\f$.
		 *
		 * Read the paper: page 542, first paragraph.
		 */
		uint64_t m_r;

		/// Number of vertices of the trees.
		uint64_t m_n;

		/// Was the last tree generated?
		bool m_is_last = false;
		/// First time calling @ref next().
		bool m_first_it = true;

};

} // -- namespace generate
} // -- namespace lal
