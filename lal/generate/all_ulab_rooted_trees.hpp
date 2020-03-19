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
#include <lal/graphs/urtree.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive unlabelled rooted tree generator.
 *
 * Generates all the unlabelled rooted trees of a given number of vertices.
 *
 * In order to use this class, you must first provide the size \f$n\f$ of the
 * tree (number of vertices) in the constructor or in the @ref init method of
 * the trees to be generated.
 *
 * Then, call method @ref next to modify the internal state that will allow the
 * user to construct the tree by calling method @ref get_tree.
 *
 * All the unlabelled rooted trees have been generated when method @ref has_next
 * returns false. At this point, method @ref get_tree will always construct a
 * star tree of \f$n\f$ vertices rooted at its central vertex.
 *
 * In order to restart the generation of these trees, call method @ref init
 * again. It is allowed, at any time, to call @ref init with the same size of
 * the trees, or with a different one.
 *
 * The algorithm implemented can be found in the work by Beyer et al. (see
 * \cite Beyer1980a). The definition of the public and private members of this
 * class follow the notation in this work.
 *
 * The correct usage of this class is
 * @code
 *		all_ulab_rooted_trees TreeGen(n);
 *		while (TreeGen.has_next()) {
 *			TreeGen.next();
 *			urtree T = TreeGen.get_tree();
 *			// ...
 *		}
 * @endcode
 */
class all_ulab_rooted_trees {
	public:
		/**
		 * @brief Default constructor.
		 *
		 * When constructed this way, the class needs to be initialised.
		 * See @ref init(uint32_t).
		 */
		all_ulab_rooted_trees();
		/// Constructor with number of vertices.
		all_ulab_rooted_trees(uint32_t n);
		/// Default destructor
		~all_ulab_rooted_trees();

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
		void init(uint32_t n);

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
		graphs::urtree get_tree() const;

	private:
		/// Number of vertices of the tree
		uint32_t m_n;

		/// Is the current tree the last tree to be generated?
		bool m_is_last = false;
		/// Is the current tree the first tree to be generated?
		bool m_is_first = false;

		/// Pointer as in the paper.
		uint32_t m_p = 0;
		/// Sequence SAVE
		std::vector<node> m_save;
		/// Sequence PREV
		std::vector<node> m_prev;
		/// Level sequence of the tree
		std::vector<node> m_L;
};

} // -- namespace generate
} // -- namespace lal
