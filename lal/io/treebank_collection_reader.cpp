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

#include <lal/io/treebank_collection_reader.hpp>

// C++ includes
#include <filesystem>
using namespace std;

namespace lal {
namespace io {

treebank_error treebank_collection_reader::init(const string& main_file) noexcept
{
	// close current dataset (if any)
	m_list.close();
	m_reached_end = false;
	m_no_more_treebanks = false;

	m_main_file = main_file;
	if (not filesystem::exists(m_main_file)) {
		return treebank_error::main_file_does_not_exist;
	}

	// open new dataset and read the first line
	m_list.open(m_main_file);
	if (not m_list.is_open()) {
		return treebank_error::main_file_could_not_be_opened;
	}

	step_line();
	next_treebank();

	return treebank_error::no_error;
}

void treebank_collection_reader::next_treebank() noexcept {
	if (m_no_more_treebanks) {
		m_reached_end = true;
		return;
	}

	// build path to treebank file
	filesystem::path M(m_main_file);
	M.replace_filename(m_cur_treebank_filename);

	// this call
	m_treebank_reader.init(M.string(), m_cur_treebank_name);
	// can only return
	//     lal::io::treebank_error::treebank_file_could_not_be_opened
	// which can be checked with 'is_open'

	step_line();
}

} // -- namespace io
} // -- namespace lal
