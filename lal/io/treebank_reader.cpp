/*********************************************************************
 *
 *  Linear Arrangement Library - A library that implements a collection
 *  algorithms for linear arrangments of graphs.
 *
 *  Copyright (C) 2019-2020
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
#include <sstream>
using namespace std;

// lal includes
#include <lal/internal/graphs/trees/convert_to_rtree.hpp>

namespace lal {
using namespace graphs;

namespace io {

// MODIFIERS

dataset_error treebank_reader::init(const string& file, const string& lang) {
	m_treebank.close();
	m_treebank_identifier = lang;
	m_treebank_file = file;

	m_treebank.open(m_treebank_file.c_str());
	if (not m_treebank.is_open()) {
		return dataset_error::treebank_could_not_be_opened;
	}
	return dataset_error::no_error;
}

bool treebank_reader::has_tree() const {
	return not m_treebank.eof();
}

dataset_error treebank_reader::next_tree() {
	getline(m_treebank, m_file_line);
	if (m_file_line.length() == 1) {
		// line is probably empty...
		if (not ('0' <= m_file_line[0] and m_file_line[0] <= '9')) {
			// line is certainly empty
			return dataset_error::empty_line;
		}
	}

	++m_num_trees;

	// this peek is needed so that treebank.eof()
	// returns true when it has to.
	m_treebank.peek();

	return dataset_error::no_error;
}

// GETTERS

rooted_tree treebank_reader::get_tree() const {
	// parse tree in line
	stringstream ss(m_file_line);

	// parent of each node
	vector<node> L;
	// read parents
	node k;
	while (ss >> k) { L.push_back(k); }

	rooted_tree t = internal::linear_sequence_to_rtree(L);
	t.calculate_size_subtrees();
	t.calculate_tree_type();
	return t;
}

} // -- namespace io
} // -- namespace lal
