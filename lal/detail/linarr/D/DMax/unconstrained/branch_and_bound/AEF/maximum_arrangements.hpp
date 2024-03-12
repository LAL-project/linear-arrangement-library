/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2024
 *
 * This file is part of Linear Arrangement Library. The full code is available
 * at:
 *      https://github.com/LAL-project/linear-arrangement-library.git
 *
 * Linear Arrangement Library is free software: you can redistribute it
 * and/or modify it under the terms of the GNU Affero General Public License
 * as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * Linear Arrangement Library is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Linear Arrangement Library.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Contact:
 *
 *     Lluís Alemany Puig (lalemany@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// C++ includes
#include <vector>
#if defined DEBUG
#include <cassert>
#endif

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/detail/linarr/level_signature.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

/**
 * @brief Set of maximum arrangements up to isomorphism.
 *
 * Implements a class that stores unique arrangements according to the definition
 * of level isomorphism (see @ref LAL_concepts__linear_arrangement__isomorphism).
 * Each unique arrangement in this class is said to be a 'representative' of its
 * isomorphism class (@ref m_representatives). Each representative is stored
 * alongside its multiplicity (@ref m_amount) and its corresponding level signature
 * (@ref m_level_signatures).
 */
class set_max_arrangements {
private:
	static constexpr level_signature_type per_vertex = level_signature_type::per_vertex;

public:
	/// Constructor bound to a free tree.
	set_max_arrangements(const graphs::free_tree& t) noexcept : m_t(t) { }

	/// Destructor
	~set_max_arrangements() noexcept { }

	/// Initialize the object.
	void init() noexcept {
		m_max_value = 0;
	}

	/* GETTERS */

	/// Returns the maximum value found so far.
	[[nodiscard]] uint64_t get_max_value() const noexcept
	{ return m_max_value; }

	/// Returns the number of representatives.
	[[nodiscard]] std::size_t get_num_representatives() const noexcept
	{ return m_representatives.size(); }

	/// Returns the set of representatives.
	[[nodiscard]] std::vector<linear_arrangement>&& retrieve_all_representatives()
	noexcept
	{
		return std::move(m_representatives);
	}

	/// Returns the multiplicity of the @e i-th representative.
	[[nodiscard]] std::size_t get_size_class(std::size_t i) const noexcept {
#if defined DEBUG
		assert(i < m_amount.size());
#endif
		return m_amount[i];
	}

	/// Returns the @e i-th representative.
	[[nodiscard]] const linear_arrangement& get_representative(std::size_t i)
	const noexcept
	{
#if defined DEBUG
		assert(i < m_representatives.size());
#endif
		return m_representatives[i];
	}

	/// Returns the level signature of the @e i-th representative.
	[[nodiscard]]
	const level_signature_per_vertex& get_level_signature(std::size_t i)
	const noexcept
	{
#if defined DEBUG
		assert(i < m_level_signatures.size());
#endif
		return m_level_signatures[i];
	}

	/* MODIFIERS */

	/**
	 * @brief Adds a new arrangement to this class.
	 *
	 * If the new arrangement is isomorphic to an already-existing representative,
	 * this simply increments its multiplicity (@ref m_amount).
	 * @param value Value of the arrangement.
	 * @param arr New arrangement.
	 */
	void add(uint64_t value, const linear_arrangement& arr) noexcept {
		if (m_max_value < value) {
			m_max_value = value;

			m_representatives.clear();
			m_level_signatures.clear();
			m_amount.clear();

			m_representatives.push_back(arr);

			level_signature_per_vertex L = calculate_level_signature<per_vertex>(m_t, arr);
			m_level_signatures.push_back(std::move(L));
			m_amount.push_back(1);
		}
		else if (m_max_value == value) {
			level_signature_per_vertex L = calculate_level_signature<per_vertex>(m_t, arr);
			const std::size_t idx_repr = find_representative(L);
			if (idx_repr == m_representatives.size()) {
				m_representatives.push_back(arr);
				m_level_signatures.push_back(std::move(L));
				m_amount.push_back(1);
			}
			else {
				++m_amount[idx_repr];
			}
		}
	}

	/**
	 * @brief Merges another set of maximum arrangements into this one.
	 * @param max_arrs Set of maximum arrangements.
	 * @post The set passed as parameter should be considered moved and thus
	 * unusable after this call.
	 */
	void merge(set_max_arrangements&& max_arrs) noexcept {
		// nothing to do
		if (m_max_value > max_arrs.m_max_value) {
			return;
		}

		// just copy the contents of 'max_arrs'
		if (m_max_value < max_arrs.m_max_value) {
			m_representatives.clear();
			m_level_signatures.clear();
			m_amount.clear();

			m_max_value = max_arrs.m_max_value;
			m_representatives = std::move(max_arrs.m_representatives);
			m_level_signatures = std::move(max_arrs.m_level_signatures);
			m_amount = std::move(max_arrs.m_amount);
			return;
		}

		// actually merge the two sets
		for (std::size_t i = 0; i < max_arrs.m_representatives.size(); ++i) {
			const linear_arrangement& arr_i = max_arrs.m_representatives[i];

			// find the representative's index in this set of arrangements
			const std::size_t idx_repr = find_representative(max_arrs.m_level_signatures[i]);

			// update collection
			if (idx_repr < m_representatives.size()) {
				++m_amount[idx_repr];
			}
			else {
				m_representatives.push_back(arr_i);
				m_level_signatures.push_back(max_arrs.m_level_signatures[i]);
				m_amount.push_back(max_arrs.m_amount[i]);
			}
		}
	}

private:
	/**
	 * @brief Find the level signature isomorphic to @e L
	 * @param L Input level signature.
	 * @returns An index in the range \f$[0,N]\f$, where \f$N\f$ equals the number
	 * of representatives in this set.
	 */
	[[nodiscard]] std::size_t find_representative
	(const level_signature_per_vertex& L)
	const noexcept
	{
		const level_signature_per_vertex mL = mirror_level_signature(L);

		// The isomorphism to use is based on 'simple' arrangement isomorphism
		for (std::size_t i = 0; i < m_representatives.size(); ++i) {

			const bool isomorphic =
				m_level_signatures[i] == L or
				m_level_signatures[i] == mL;

			if (isomorphic) { return i; }
		}

		return m_representatives.size();
	}

private:
	/// The tree for which the arrangements are stored.
	const graphs::free_tree& m_t;

	/// Maximum value found.
	uint64_t m_max_value;
	/// List of representative arrangements.
	std::vector<linear_arrangement> m_representatives;
	/// List of level signatures per representatives.
	std::vector<level_signature_per_vertex> m_level_signatures;
	/// Multiplicities of each representative.
	std::vector<uint64_t> m_amount;
};

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
