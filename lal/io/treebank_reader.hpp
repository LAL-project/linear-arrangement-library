/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019 - 2021
 *
 *  This file is part of Linear Arrangement Library. The full code is available
 *  at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
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
#include <fstream>
#include <string>
#include <vector>

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/io/treebank_error.hpp>

namespace lal {
namespace io {

/**
 * @brief A reader for a single treebank file.
 *
 * This class, the objects of which will be referred to as the "readers", offers
 * a simple interface for iterating over the trees in a single treebank file,
 * henceforth referred to as the treebank (see @ref LAL_concepts__treebank for
 * further details on treebank files).
 *
 * In order to use it, this class has to be first initialized with the treebank
 * file and, optionally, a self-descriptive string, i.e., something that identifies
 * the treebank (e.g., an ISO code of a language). Once initialised, the first
 * tree can be retrievend with @ref get_tree. The other trees can be iterated over
 * by calling @ref next_tree. This function can only be called as long as @ref end
 * returns false.
 *
 * If an object of this class was returned by the class @ref treebank_collection_reader,
 * then methods @ref get_treebank_filename and @ref get_treebank_identifier might
 * prove useful for debugging since they return, respectively, the full name
 * (path included) of the treebank and an identifier string.
 *
 * An example of usage of this class is given in the following piece of code.
 * @code
 *		lal::io::treebank_reader tbread;
 *		// it is advisable to check for errors
 *		const auto err = tbread.init(main_file);
 *		while (not tbread.end()) {
 *			const lal::graphs::rooted_tree t = tbread.get_tree();
 *			// process tree 't'
 *			// ....
 *			tbread.next_tree();
 *		}
 * @endcode
 */
class treebank_reader {
public:
	// MODIFIERS

	/**
	 * @brief Initialises the treebank reader
	 * @param file Treebank file.
	 * @param identifier Identifier string for the treebank.
	 * @returns The type of the error, if any. The list of errors that this
	 * method can return is:
	 * - @ref lal::io::treebank_error_type::treebank_file_could_not_be_opened
	 * @post The amount of trees processed, @ref m_num_trees, is always
	 * set to 0.
	 */
	treebank_error init
	(const std::string& file, const std::string& identifier = "") noexcept;

	/// Returns whether there is another tree to be processed.
	bool end() const noexcept { return m_no_more_trees; }

	/**
	 * @brief Retrieves the next tree in the file.
	 * @post Increments the amount of trees found.
	 */
	void next_tree() noexcept;

	/* GETTERS */

	/**
	 * @brief Returns the number of trees processed so far.
	 *
	 * When method @ref end returns 'true', this method returns the exact
	 * amount of trees in the treebank.
	 */
	std::size_t get_num_trees() const noexcept
	{ return m_num_trees; }

	/// Returns the identifier corresponding of the treebank.
	const std::string& get_treebank_identifier() const noexcept
	{ return m_treebank_identifier; }

	/// Returns the name of the treebank file.
	const std::string& get_treebank_filename() const noexcept
	{ return m_treebank_file; }

	/// Returns the current tree.
	graphs::rooted_tree get_tree() const noexcept;

	/// Returns the current head vector.
	head_vector get_head_vector() const noexcept
	{ return m_current_head_vector; }

	/**
	 * @brief Can the treebank be read?
	 *
	 * If the @ref init method returned an error different from
	 * @ref lal::io::treebank_error_type::no_error then this returns false.
	 * @return Whether the treebank is readable or not.
	 */
	bool is_open() const noexcept { return m_treebank.is_open(); }

	/* SETTERS */

	/**
	 * @brief Should trees be normalised?
	 * @param v Boolean value.
	 */
	void set_normalise(bool v) noexcept
	{ m_normalise_tree = v; }

	/**
	 * @brief Should the size of the subtrees be calculated?
	 * @param v Boolean value.
	 */
	void set_calculate_size_subtrees(bool v) noexcept
	{ m_calculate_size_subtrees = v; }

	/**
	 * @brief Should the tree be classified into types?
	 *
	 * See @ref lal::graphs::tree_type for details on the classification.
	 * @param v Boolean value.
	 */
	void set_calculate_tree_type(bool v) noexcept
	{ m_calculate_tree_type = v; }

	/**
	 * @brief Set this treebank's identifier string.
	 *
	 * This method overrides the contents of @ref m_treebank_identifier.
	 * This method is most useful when, <b>after initialising</b> a treebank
	 * reader, the identifier string is to be changed in some way.
	 * @param id Identifier string.
	 */
	void set_identifier(const std::string& id) noexcept
	{ m_treebank_identifier = id; }

private:
	/// Identifier for the treebank.
	std::string m_treebank_identifier = "none";
	/// Treebank's file name (with the full path).
	std::string m_treebank_file = "none";
	/// Handler for main file reading.
	std::ifstream m_treebank;

	/// Number of trees in the treebank.
	std::size_t m_num_trees = 0;
	/// Current line.
	std::string m_current_line;
	/// Current head vector
	head_vector m_current_head_vector;

	/// Normalise the current tree.
	bool m_normalise_tree = true;
	/// Calculate the size of the subtrees of the generated rooted tree.
	bool m_calculate_size_subtrees = true;
	/// Calculate the type of tree of the generated tree.
	bool m_calculate_tree_type = true;
	/// Have all trees in the file been consumed?
	bool m_no_more_trees = false;
};

} // -- namespace io
} // -- namespace lal
