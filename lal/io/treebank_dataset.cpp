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

#include <lal/io/treebank_dataset.hpp>

// C++ includes
#include <filesystem>
#include <sstream>
using namespace std;

// lal includes
#include <lal/utils/graphs/trees/conversions.hpp>

namespace lal {
namespace io {

treebank_dataset::treebank_dataset() {
	m_main_list = "none";
}

treebank_dataset::~treebank_dataset() {}

dataset_error treebank_dataset::init(const string& main_file)
{
	// close current dataset (if any)
	m_list.close();

	m_main_list = main_file;
	if (not filesystem::exists(m_main_list)) {
		return dataset_error::missing_main_file;
	}

	// open new dataset
	m_list.open(m_main_list);
	if (m_list >> m_lang >> m_tbf) {
		// do nothing, there are more trees
	}
	else {
		m_lang = m_tbf = "none";
	}
	return dataset_error::no_error;
}

bool treebank_dataset::has_language() const {
	return m_lang != "none";
	//return not list.eof();
}

dataset_error treebank_dataset::next_language() {
	dataset_error dserr = m_tree_read.init(m_tbf, m_lang);
	if (dserr != dataset_error::no_error) {
		return dserr;
	}

	if (m_list >> m_lang >> m_tbf) {
		// do nothing, there are more trees
	}
	else {
		m_lang = m_tbf = "none";
	}
	return dataset_error::no_error;
}

treebank_reader& treebank_dataset::get_treebank_reader() {
	return m_tree_read;
}

} // -- namespace io
} // -- namespace lal
