/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2023
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
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LARCA (Laboratory for Relational Algorithmics, Complexity and Learning)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office S124, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

#pragma once

// lal includes
#include <lal/linear_arrangement.hpp>
#include <lal/numeric/rational.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/graphs/free_tree.hpp>

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
	uint64_t get_left_span() const noexcept { return m_left_span;; }
	/// Returns right span of this flux.
	uint64_t get_right_span() const noexcept { return m_right_span; }
	/// Returns the size of this flux.
	uint64_t get_size() const noexcept
	{ return m_dependencies.size(); }
	/// Returns weight of this flux.
	uint64_t get_weight() const noexcept { return m_weight; }

	/// Returns a reference to the left span of this flux.
	uint64_t& get_left_span() noexcept { return m_left_span;; }
	/// Returns a reference to the right span of this flux.
	uint64_t& get_right_span() noexcept { return m_right_span; }
	/// Returns a reference to the weight of this flux.
	uint64_t& get_weight() noexcept { return m_weight; }

	/// Returns a reference to the set of dependencies.
	std::vector<edge>& get_dependencies() { return m_dependencies; }

	/// Returns the set of dependencies.
	const std::vector<edge>& get_dependencies() const noexcept
	{ return m_dependencies; }

	/**
	 * @brief Returns the R/L ratio.
	 *
	 * This is the right span (see attribute @ref m_right_span) divided by the
	 * left span (see attribute @ref m_left_span).
	 * @returns The R/L ratio of this flux.
	 */
	numeric::rational get_RL_ratio_rational() const noexcept
	{ return numeric::rational(m_right_span, m_left_span); }
	/**
	 * @brief Returns the R/L ratio.
	 *
	 * This is the right span (see attribute @ref m_right_span) divided by the
	 * left span (see attribute @ref m_left_span).
	 * @returns The R/L ratio of this flux.
	 */
	double get_RL_ratio() const noexcept
	{ return get_RL_ratio_rational().to_double(); }

	/**
	 * @brief Returns the W/S ratio.
	 *
	 * Also known as @e density, this is the weight (see attribtue @ref m_weight)
	 * divided by the number of dependencies (see attribute @ref m_dependencies).
	 * @returns The W/S ratio of this flux.
	 */
	numeric::rational get_WS_ratio_rational() const noexcept
	{ return numeric::rational(m_weight, m_dependencies.size()); }
	/**
	 * @brief Returns the W/S ratio.
	 *
	 * Also known as @e density, this is the weight (see attribtue @ref m_weight)
	 * divided by the number of dependencies (see attribute @ref m_dependencies).
	 * @returns The W/S ratio of this flux.
	 */
	double get_WS_ratio() const noexcept
	{ return get_WS_ratio_rational().to_double(); }

	// SETTERS

	/// Sets the left span.
	void set_left_span(uint64_t ls) noexcept { m_left_span = ls; }
	/// Sets the right span.
	void set_right_span(uint64_t rs) noexcept { m_right_span = rs; }
	/// Sets the weight.
	void set_weight(uint64_t w) noexcept { m_weight = w; }

	/// Sets the set of dependencies.
	void set_dependencies(const std::vector<edge>& deps) noexcept
	{ m_dependencies = deps; }

	/// Sets the set of dependencies.
	void set_dependencies(std::vector<edge>&& deps) noexcept
	{ m_dependencies = std::move(deps); }

private:
	/// Number of words to the left of this flux which are vertices of a
	/// dependency in the flux.
	uint64_t m_left_span = 0;
	/// Number of words to the right of this flux which are vertices of a
	/// dependency in the flux.
	uint64_t m_right_span = 0;
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
	uint64_t m_weight = 0;
};

/**
 * @brief Computes the flux of a dependency tree.
 *
 * This function is implemented based on the explanations given in \cite Kahane2017a.
 * @param t Input free tree (or dependency tree).
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The set of dependency fluxes in the arrangement.
 * @pre The tree @e t is a valid free tree. Method graphs::free_tree::is_tree
 * returns true.
 */
std::vector<dependency_flux>
compute_flux(const graphs::free_tree& t, const linear_arrangement& pi = {}) noexcept;

/**
 * @brief Computes the flux of a dependency tree.
 *
 * This function is implemented based on the explanations given in \cite Kahane2017a.
 * @param t Input rooted tree (or dependency tree).
 * @param pi A linear arrangement of the nodes. When omitted, \f$\pi_I\f$ is used.
 * @returns The set of dependency fluxes in the arrangement.
 * @pre The tree @e t is a valid rooted tree. Method graphs::rooted_tree::is_rooted_tree
 * returns true.
 */
inline
std::vector<dependency_flux> compute_flux
(const graphs::rooted_tree& t, const linear_arrangement& pi = {}) noexcept
{
#if defined DEBUG
	assert(t.is_rooted_tree());
#endif
	return compute_flux(t.to_free_tree(), pi);
}


} // -- namespace linarr
} // -- namespace lal
