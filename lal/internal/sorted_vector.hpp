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
#if defined DEBUG
#include <cassert>
#endif
#include <algorithm>
#include <vector>

namespace lal {

template<class T>
class sorted_vector : public std::vector<T> {
public:
	sorted_vector() : std::vector<T>() { }
	sorted_vector(std::size_t n) : std::vector<T>(n) { }
	sorted_vector(std::size_t n, const T& x) : std::vector<T>(n, x) { }

	sorted_vector(const sorted_vector<T>& v) : std::vector<T>(v) { }
	sorted_vector(sorted_vector<T>&& v) : std::vector<T>(std::move(v)) { }

	sorted_vector& operator= (const sorted_vector& v) { *this = v; }
	sorted_vector& operator= (sorted_vector&& v) { *this = std::move(v); }

	~sorted_vector() { }

	// insert sorted, with copies
	inline typename std::vector<T>::iterator
	insert_sorted(const T& x) {
		const auto it = std::upper_bound(this->begin(), this->end(), x);
		return this->insert(it, x);
	}
	inline typename std::vector<T>::iterator
	insert_sorted(T&& x) {
		const auto it = std::upper_bound(this->begin(), this->end(), x);
		return this->insert(it, std::move(x));
	}

	// insert sorted, no copies (if the element already exists, do not insert)
	inline typename std::vector<T>::iterator
	insert_sorted_unique(const T& x) {
		if (this->size() == 0) {
			this->push_back(x);
			return this->begin();
		}

		const auto it = upper_bound(this->begin(), this->end(), x);
		const auto pre_it = it == this->begin() ? it : it - 1;
		if (*pre_it != x) {
			return this->insert(it, x);
		}
		return it;
	}
	inline typename std::vector<T>::iterator
	insert_sorted_unique(T&& x) {
		if (this->size() == 0) {
			this->push_back(std::move(x));
			return this->begin();
		}

		const auto it = std::upper_bound(this->begin(), this->end(), x);
		const auto pre_it = it == this->begin() ? it : it - 1;
		if (*pre_it != x) {
			return this->insert(it, std::move(x));
		}
		return it;
	}

	// remove an element, assuming that the element is sorted
	inline typename std::vector<T>::iterator
	remove_sorted(const T& x) {
#if defined DEBUG
		assert(contains(x));
#endif
		const auto i1 = std::lower_bound(this->begin(), this->end(), x);
		return this->erase(i1);
	}

	// returns whether the vector contains an element
	inline bool contains(const T& x) const {
		return std::binary_search(this->begin(), this->end(), x);
	}

	// find the position of an element in logarithmic time
	inline typename std::vector<T>::iterator
	find_sorted(const T& x) {
		const auto i = std::lower_bound(this->begin(), this->end(), x);
		if (i == this->end()) { return this->end(); }

		if (*i == x) { return i; }
		return this->end();
	}
};

} // -- namespace lal
