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

#include <lal/io/basic_output.hpp>

// C++ includes
using namespace std;

namespace lal {
using namespace graphs;
using namespace numeric;

ostream& operator<< (ostream& os, const graph& g)
{
   const uint32_t N = g.n_nodes();
   for (node u = 0; u < N; ++u) {
	   os << u << ":";
	   for (auto v : g.get_neighbours(u)) {
		   os << " " << v;
	   }
	   os << (u < N - 1 ? "\n" : "");
   }
   return os;
}

ostream& operator<< (ostream& os, const urtree& g) {
	const uint32_t N = g.n_nodes();
	const string pad = (g.has_root() ? " " : "");
	for (node u = 0; u < N; ++u) {
		os << (u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

ostream& operator<< (ostream& os, const drtree& g) {
	const uint32_t N = g.n_nodes();
	const string pad = (g.has_root() ? " " : "");
	for (node u = 0; u < N; ++u) {
		os << (u == g.get_root() ? "*" : pad) << u << ":";
		for (auto v : g.get_neighbours(u)) {
			os << " " << v;
		}
		os << (u < N - 1 ? "\n" : "");
	}
	return os;
}

ostream& operator<< (ostream& os, const integer& i) {
	os << i.to_string();
	return os;
}

ostream& operator<< (ostream& os, const rational& r) {
	os << r.to_string();
	return os;
}

} // -- namespace lal
