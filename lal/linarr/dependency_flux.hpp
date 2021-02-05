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

#pragma once

// lal includes
#include <lal/definitions.hpp>

namespace lal {
namespace linarr {

/**
 * @brief Dependency flux.
 *
 * The dependency flux of a dependency tree, as defined in \cite Kahane2017a.
 */
class dependency_flux {
public:

	// GETTERS

	/// Returns left span of this flux.
	inline uint32_t get_left_span() const { return m_left_span;; }
	/// Returns right span of this flux.
	inline uint32_t get_right_span() const { return m_right_span; }
	/// Returns the size of this flux.
	inline uint32_t get_size() const
	{ return static_cast<uint32_t>(m_dependencies.size()); }
	/// Returns weight of this flux.
	inline uint32_t get_weight() const { return m_weight; }
	/// Returns whether this flux is a left-branching bouquet or not.
	inline bool is_left_bouquet() const { return m_is_left_bouquet; }
	/// Returns whether this flux is a right-branching bouquet or not.
	inline bool is_right_bouquet() const { return m_is_right_bouquet; }

#ifndef SWIG
	/// Returns a reference to the left span of this flux.
	inline uint32_t& get_left_span() { return m_left_span;; }
	/// Returns a reference to the right span of this flux.
	inline uint32_t& get_right_span() { return m_right_span; }
	/// Returns a reference to the weight of this flux.
	inline uint32_t& get_weight() { return m_weight; }
	/// Returns a reference to whether this flux is a left-branching bouquet or not.
	inline bool& is_left_bouquet() { return m_is_left_bouquet; }
	/// Returns a reference to whether this flux is a right-branching bouquet or not.
	inline bool& is_right_bouquet() { return m_is_right_bouquet; }

	/// Returns a reference to the set of dependencies.
	std::vector<edge>& get_dependencies() { return m_dependencies; }
#endif

	/// Returns the set of dependencies.
	const std::vector<edge>& get_dependencies() const { return m_dependencies; }

	// SETTERS

	/// Sets the left span.
	inline void set_left_span(uint32_t ls) { m_left_span = ls; }
	/// Sets the right span.
	inline void set_right_span(uint32_t rs) { m_right_span = rs; }
	/// Sets the weight.
	inline void set_weight(uint32_t w) { m_weight = w; }
	/// Sets whether this flux is a left-branching bouquet or not.
	inline void set_left_bouquet(bool lb) { m_is_left_bouquet = lb; }
	/// Sets whether this flux is a right-branching bouquet or not.
	inline void set_right_bouquet(bool rb) { m_is_right_bouquet = rb; }

	/// Sets the set of dependencies.
	inline void set_dependencies(const std::vector<edge>& deps)
	{ m_dependencies = deps; }

#ifndef SWIG
	/// Sets the set of dependencies.
	inline void set_dependencies(std::vector<edge>&& deps)
	{ m_dependencies = std::move(deps); }
#endif

private:
	/// Number of words to the left of this position.
	uint32_t m_left_span = 0;
	/// Number of words to the right of this position.
	uint32_t m_right_span = 0;
	/**
	 * @brief Dependencies in this flux.
	 *
	 * The set of concomitant dependencies.
	 */
	std::vector<edge> m_dependencies;
	/**
	 * @brief Weight of this flux.
	 *
	 * This is the size of the largest subset of disjoint dependencies in
	 * @ref m_dependencies.
	 */
	uint32_t m_weight = 0;

	/**
	 * @brief Is this flux a left-branching bouquet?
	 *
	 * A bouquet is a set of dependencies sharing the same vertex. It is
	 * left-branching if the common vertex is on the right.
	 */
	bool m_is_left_bouquet = false;
	/**
	 * @brief Is this flux a right-branching bouquet?
	 *
	 * A bouquet is a set of dependencies sharing the same vertex. It is
	 * right-branching if the common vertex is on the left.
	 */
	bool m_is_right_bouquet = false;
};

} // -- namespace linarr
} // -- namespace lal
