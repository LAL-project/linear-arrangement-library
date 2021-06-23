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
 
#include <lal/generate/all_ulab_free_trees.hpp>

// C++ includes
#include <limits>
using namespace std;

// lal includes
#include <lal/internal/graphs/conversions.hpp>

#define inf numeric_limits<uint64_t>::max()

namespace lal {
using namespace graphs;

namespace generate {

/* PUBLIC */

/* CONSTRUCTORS */

all_ulab_free_trees::all_ulab_free_trees(uint64_t _n) noexcept
	: _tree_generator<free_tree>(_n),
	  m_L(m_n + 1),
	  m_W(m_n + 1)
{
	reset();
}

/* MODIFIERS */

void all_ulab_free_trees::next() noexcept {
	if (m_is_last or m_reached_end) {
		m_reached_end = true;
		return;
	}

	if (m_n <= 2) {
		m_is_last = true;
		return;
	}
	if (m_first_it) {
		m_first_it = false;
		return;
	}

	// implementation copied "as is" from the cited paper
	// in the description of the class. Read the paper...

	const bool cond1 =
		(m_c == m_n + 1) or
		(m_p == m_h2 and (
			(m_L[m_h1] == m_L[m_h2] + 1 and m_n - m_h2 > m_r - m_h1)
			or
			(m_L[m_h1] == m_L[m_h2] and m_n - m_h2 + 1 < m_r - m_h1)
		));

	const bool fixit = cond1 and (m_L[m_r] > 3);

	if (cond1) {
		if (m_L[m_r] > 3) {
			m_p = m_r;
			m_q = m_W[m_r];

			// --
			//if (m_h1 == m_r) { --m_h1; }
			m_h1 = m_h1 - (m_h1 == m_r ? 1 : 0);
			// --

			// variable 'fixit' is set in a few lines above this.
			//fixit = true;
		}
		else {
			m_p = m_r;
			--m_r;
			m_q = 2;
		}
	}

	bool needr, needc, needh2;
	needr = needc = needh2 = false;

	// --
	//if (m_p <= m_h1) { m_h1 = m_p - 1; }
	m_h1 = (m_p <= m_h1 ? m_p - 1 : m_h1);
	// --

	if (m_p <= m_r) {
		needr = true;
	}
	else if (m_p <= m_h2) {
		needh2 = true;
	}
	else if (m_L[m_h2] == m_L[m_h1] - 1 and m_n - m_h2 == m_r - m_h1) {
		//if (m_p <= m_c) { needc = true; }
		needc = (m_p <= m_c ? true : needc);
	}
	else {
		m_c = inf;
	}

	const uint64_t oldp = m_p;
	const uint64_t delta = m_q - m_p;
	const uint64_t oldLq = m_L[m_q];
	const uint64_t oldWq = m_W[m_q];
	m_p = inf;

	for (uint64_t i = oldp; i <= m_n; ++i) {
		m_L[i] = m_L[i + delta];
		if (m_L[i] == 2) {
			m_W[i] = 1;
		}
		else {
			m_p = i;

			// --
			/*if (m_L[i] == oldLq) { m_q = oldWq; }
			else { m_q = m_W[i + delta] - delta; }*/
			m_q = (m_L[i] == oldLq ? oldWq : m_W[i + delta] - delta);
			// --

			m_W[i] = m_q;
		}

		// --
		/*if (needr and m_L[i] == 2) {
			needr = false;
			needh2 = true;
			m_r = i - 1;
		}*/
		m_r = (needr and m_L[i] == 2 ? i - 1 : m_r);
		needh2 = (needr and m_L[i] == 2 ? true : needh2);
		needr = (needr and m_L[i] == 2 ? false : needr);
		// --

		if (needh2 and m_L[i] <= m_L[i - 1] and i > m_r + 1) {
			needh2 = false;
			m_h2 = i - 1;
			if (m_L[m_h2] == m_L[m_h1] - 1 and m_n - m_h2 == m_r - m_h1) {
				needc = true;
			}
			else {
				m_c = inf;
			}
		}
		if (needc) {

			// --
			/*if (m_L[i] != m_L[m_h1 - m_h2 + i] - 1) {
				needc = false;
				m_c = i;
			}
			else {
				m_c = i + 1;
			}*/
			needc = m_L[i] == m_L[m_h1 - m_h2 + i] - 1;
			m_c = (m_L[i] != m_L[m_h1 - m_h2 + i] - 1 ? i : i + 1);
			// --
		}
	}
	if (fixit) {
		m_r = m_n - m_h1 + 1;
		for (uint64_t i = m_r + 1; i <= m_n; ++i) {
			m_L[i] = i - m_r + 1;
			m_W[i] = i - 1;
		}
		m_W[m_r + 1] = 1;
		m_h2 = m_n;
		m_p = m_n;
		m_q = m_p - 1;
		m_c = inf;
	}
	else {
		if (m_p == inf) {

			// --
			/*if (m_L[oldp - 1] != 2) { m_p = oldp - 1; }
			else { m_p = oldp - 2; }*/
			m_p = (m_L[oldp - 1] != 2 ? oldp - 1 : oldp - 2);
			// --

			m_q = m_W[m_p];
		}
		if (needh2) {
			m_h2 = m_n;

			// --
			/*if (m_L[m_h2] == m_L[m_h1] - 1 and m_h1 == m_r) { m_c = m_n + 1; }
			else { m_c = inf; }*/
			m_c = (m_L[m_h2] == m_L[m_h1] - 1 and m_h1 == m_r ? m_n + 1 : inf);
			// --
		}
	}

	m_is_last = (m_q == 0);
}

void all_ulab_free_trees::__reset() noexcept {
	m_is_last = false;
	m_reached_end = false;

	m_L.fill(0);
	m_W.fill(0);

	// simplest cases
	if (m_n == 0) {
		m_is_last = true;
		return;
	}
	if (m_n <= 2) {
		m_is_last = false;
		return;
	}

	uint64_t k = m_n/2 + 1;
	m_p = (m_n == 4 ? 3 : m_n);
	m_q = m_n - 1;
	m_h1 = k;
	m_h2 = m_n;
	m_r = k;
	m_c = (m_n%2 == 0 ? m_n + 1 : inf);

	// initialise L and W
	for (uint64_t i = 1; i <= k; ++i) {
		m_W[i] = i - 1;
		m_L[i] = i;
	}
	m_W[k+1] = 1;
	m_L[k+1] = 2;
	for (uint64_t i = k + 2; i <= m_n; ++i) {
		m_W[i] = i - 1;
		m_L[i] = i - k + 1;
	}

	//if (m_n <= 3) { m_first_it = false; }
	m_first_it = m_n > 3;
}

/* PROTECTED */

free_tree all_ulab_free_trees::__get_tree() noexcept {
	if (m_n <= 1) { return free_tree(m_n); }
	if (m_n == 2) {
		free_tree t(2);
		t.add_edge(0,1);
		return t;
	}
	return internal::level_sequence_to_ftree(m_L.begin(), m_n, false, false);
}

} // -- namespace generate
} // -- namespace lal

