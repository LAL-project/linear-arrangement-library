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
 
#include <lal/io/treebank_reader.hpp>

// C++ includes
#include <iostream>
#include <sstream>
using namespace std;

// lal includes
#include <lal/graphs/conversions.hpp>

namespace lal {
using namespace graphs;

namespace io {

// MODIFIERS

treebank_error treebank_reader::init
(const string& file, const string& lang) noexcept {
	m_treebank.close();
	m_treebank_identifier = lang;
	m_treebank_file = file;
	m_num_trees = 0;

	m_treebank.open(m_treebank_file.c_str());
	if (not m_treebank.is_open()) {
		return treebank_error::treebank_file_could_not_be_opened;
	}
	return treebank_error::no_error;
}

treebank_error treebank_reader::next_tree() noexcept {
	m_current_head_vector.clear();

	getline(m_treebank, m_file_line);
	if (m_file_line.length() == 1) {
		// line is probably empty...
		const bool is_digit = '0' <= m_file_line[0] and m_file_line[0] <= '9';
		const bool is_eol = m_file_line[0] == '\n';
		if (not is_digit or is_eol) {
			// we consider the line to be empty (either with an endline or
			// any other non-numeric character)
			return treebank_error::empty_line_found;
		}
	}
	else if (m_file_line.length() == 0) {
		// this is an actual empty line!
		return treebank_error::empty_line_found;
	}

	// construct the current head vector
	{
	stringstream ss(m_file_line);
	size_t count = 0;
	node k;
	while (ss >> k) {
		m_current_head_vector.push_back(k);
		++count;
	}
	if (count == 0) {
		// another actual empty line...
		return treebank_error::empty_line_found;
	}
	}

	// for statistics
	++m_num_trees;

	// this peek is needed so that treebank.eof()
	// returns true when it has to.
	m_treebank.peek();

	return treebank_error::no_error;
}

// GETTERS

rooted_tree treebank_reader::get_tree() const noexcept {
	rooted_tree t =
		from_head_vector_to_rooted_tree(
			m_current_head_vector,
			m_normalise_tree,
			true
		);

	if (m_calculate_size_subtrees) { t.calculate_size_subtrees(); }
	if (m_calculate_tree_type) { t.calculate_tree_type(); }
	return t;
}

} // -- namespace io
} // -- namespace lal
