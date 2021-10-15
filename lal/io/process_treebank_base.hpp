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

// C++ includes
#include <array>

// lal includes
#include <lal/io/treebank_feature.hpp>

namespace lal {
namespace io {

/**
 * @brief The @e processor base class.
 *
 * <b>Users should refrain from using this class.</b>
 * This class acts as a base class for all processor classes, namely, the
 * @ref lal::io::treebank_processor and the @ref lal::io::treebank_collection_processor.
 */
class _process_treebank_base {
public:
	// MODIFIERS

	/**
	 * @brief Adds a feature to the processor.
	 * @param fs Feature to be added.
	 */
	void add_feature(const treebank_feature& fs) noexcept
	{ m_what_fs[ static_cast<size_t>(fs) ] = true; }
	/**
	 * @brief Removes a feature from the processor.
	 * @param fs Feature to be removed.
	 */
	void remove_feature(const treebank_feature& fs) noexcept
	{ m_what_fs[ static_cast<size_t>(fs) ] = false; }

	// SETTERS

	/// Should the treebank file or collection be checked for errors prior to processing?
	void set_check_before_process(bool v) noexcept
	{ m_check_before_process = v; }

	/// Clear the features in the processor.
	void clear_features() noexcept
	{ std::fill(m_what_fs.begin(), m_what_fs.end(), false); }

	/**
	 * @brief Sets the separator character.
	 *
	 * The default seprator is a tabulator character '\\t'.
	 * @param c The separator character.
	 */
	void set_separator(char c) noexcept { m_separator = c; }
	/**
	 * @brief Sets the level of verbosity of the process methods.
	 *
	 * Default is 0 (i.e., no verbosity at all).
	 * Verbosity is classified by levels:
	 * - Level 1: outputs progress messages.
	 * - Level 2: outputs error messages.
	 * @param k Verbosity level.
	 */
	void set_verbosity(int k) noexcept { m_be_verbose = k; }
	/**
	 * @brief Output a hedaer for the treebank result file.
	 *
	 * Default is true.
	 * @param h Output header or not.
	 */
	void set_output_header(bool h) noexcept { m_output_header = h; }

	/**
	 * @brief Sets a custom name for the column corresponding to a given feature.
	 *
	 * This does not work for features
	 * - @ref lal::io::treebank_feature::tree_type
	 * - @ref lal::io::treebank_feature::syntactic_dependency_structure_class
	 * @param tf Feature whose column name is to be modified.
	 * @param name Custom name for the column.
	 */
	void set_column_name(const treebank_feature& tf, const std::string& name)
	noexcept
	{
		m_column_names[static_cast<std::size_t>(tf)] = name;
	}

	// GETTERS

	/**
	 * @brief Is a given feature to be calculated?
	 * @param fs The feature being queried.
	 * @returns True or False depending on whether the feature was added or removed.
	 */
	bool has_feature(const treebank_feature& fs) const noexcept
	{ return m_what_fs[ static_cast<size_t>(fs) ]; }

protected:
	/// String for each column.
	std::array<std::string, __treebank_feature_size> m_column_names;
	/// The list of features to be computed.
	std::array<bool, __treebank_feature_size> m_what_fs;

	/// Process the treebank file or collection prior to processing.
	bool m_check_before_process = true;
	/// Character used as separator
	char m_separator = '\t';
	/// Output a header for each file
	bool m_output_header = true;

	/**
	 * @brief The verbosity of the processor.
	 *
	 * When set to a value greater than or equal to 1, method process
	 * will output progress messages.
	 */
	int m_be_verbose = 0;

protected:
	/// Initialises column names @ref m_column_names
	void initialise_column_names() noexcept {
		for (std::size_t i = 0; i < __treebank_feature_size; ++i) {
			m_column_names[i] = treebank_feature_index_to_string(i);
		}
	}
};

} // -- namespace io
} // -- namespace lal
