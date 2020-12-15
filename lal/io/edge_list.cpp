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

// C++ includes
#include <filesystem>
#include <fstream>
using namespace std;

// lal includes
#include <lal/definitions.hpp>
#include <lal/graphs/undirected_graph.hpp>
#include <lal/graphs/directed_graph.hpp>

namespace lal {
using namespace graphs;

namespace io {

template<class G>
inline bool __read_edge_list(
	const string& filename, G& g, bool all, bool norm, bool check
)
{
	if (not filesystem::exists(filename)) {
		return false;
	}

	ifstream fin;
	fin.open(filename);

	vector<edge> edge_list;
	node max_vert_idx = 0;

	node u, v;
	while (fin >> u >> v) {
		edge_list.push_back(edge(u, v));
		max_vert_idx = std::max(max_vert_idx, u);
		max_vert_idx = std::max(max_vert_idx, v);
	}
	fin.close();

	g.init(max_vert_idx + 1);
	if (all) {
		g.set_edges(edge_list, norm, check);
	}
	else {
		g.set_edges(edge_list, norm, check);
	}
	return true;
}

bool read_edge_list(
	const string& filename, undirected_graph& g, bool all, bool norm, bool check
)
{
	return __read_edge_list(filename, g, all, norm, check);
}

bool read_edge_list(
	const string& filename, directed_graph& g, bool all, bool norm, bool check
)
{
	return __read_edge_list(filename, g, all, norm, check);
}

} // -- namespace io
} // -- namespace lal
