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

// C++ includes
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linarr/1level.hpp>

namespace lal {
namespace linarr {

template<class G>
numeric::rational MDD_1level_rational
(const std::vector<G>& Gs, const std::vector<LINARR>& pis)
{
#if defined DEBUG
	// the number of graphs and number of linear arrangements
	// must coincide unless no arrangement was given.
	assert(pis.size() == 0 or Gs.size() == pis.size());
#endif

	uint32_t sumD = 0;
	uint32_t sumM = 0;
	if (pis.size() == 0) {
		const LINARR empty_arr;
		for (size_t i = 0; i < Gs.size(); ++i) {
			sumD += sum_length_edges(Gs[i], empty_arr);
			sumM += Gs[i].n_edges();
		}
	}
	else {
		for (size_t i = 0; i < Gs.size(); ++i) {
			sumD += sum_length_edges(Gs[i], pis[i]);
			sumM += Gs[i].n_edges();
		}
	}
	return numeric::rational_from_ui(sumD, sumM);
}

template<class G>
double MDD_1level(const std::vector<G>& Gs, const std::vector<LINARR>& pis) {
	return MDD_1level_rational(Gs, pis).to_double();
}

} // -- namespace linarr
} // -- namespace lal
