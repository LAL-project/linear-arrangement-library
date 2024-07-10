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
 *     Lluís Alemany Puig (lluis.alemany.puig@upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/lalemany/
 *
 *     Juan Luis Esteban (esteban@cs.upc.edu)
 *         LOGPROG: Logics and Programming Research Group
 *         Office 110, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://www.cs.upc.edu/~esteban/
 *
 *     Ramon Ferrer i Cancho (rferrericancho@cs.upc.edu)
 *         LQMC (Quantitative, Mathematical, and Computational Linguisitcs)
 *         CQL (Complexity and Quantitative Linguistics Lab)
 *         Office 220, Omega building
 *         Jordi Girona St 1-3, Campus Nord UPC, 08034 Barcelona.   CATALONIA, SPAIN
 *         Webpage: https://cqllab.upc.edu/people/rferrericancho/
 *
 ********************************************************************/

// C++ includes
#if defined DEBUG
#include <cassert>
#if defined __LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/graphs/output.hpp>

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

void AEF_BnB::propagate_LV__antenna__from_leaf(const node u) noexcept {
#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Found a trigger leaf for an antenna u= " << u << '\n';
	std::cout << tab() << "    Of level value= " << m_node_level[u] << '\n';
#endif

#if defined DEBUG
	assert(m_t.get_degree(u) == 1);
#endif

	m_predicted_LV[u] = m_node_level[u];
	m_predicted_LV__origin[u] = LV_propagation_origin::self;

	const int64_t sign_u = m_node_level[u];

	const std::size_t path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];
	const std::size_t N = path.get_num_nodes();

	uint64_t num_p2 = 0;
	uint64_t num_m2 = 0;

	const node h1 = path.get_h1();
	const node h2 = path.get_h2();
	if (h1 == u) {

		int64_t sign = -sign_u;
		for (std::size_t i = 1; i < N - 1; ++i) {
			const node v = path[i];
			const int64_t prediction = 2*sign;

#if defined DEBUG
			assert(m_t.get_degree(v) == 2);
			assert(not has_valid_LV_prediction(v));
#endif

			m_predicted_LV[v] = prediction;
			m_predicted_LV__origin[v] = LV_propagation_origin::antenna_leaf;
			num_p2 += (prediction == +2);
			num_m2 += (prediction == -2);
			sign *= -1;
		}

		if (m_t.get_degree(h2) == 1) {
			const int64_t prediction = sign;
#if defined DEBUG
			assert(not has_valid_LV_prediction(h2));
#endif
			m_predicted_LV[h2] = prediction;
			m_predicted_LV__origin[h2] = LV_propagation_origin::antenna_leaf;
		}
	}
	else {
#if defined DEBUG
		assert(u == h2);
		assert(path[N - 1] == u);
#endif

		int64_t sign = -sign_u;
		for (std::size_t i = N - 2; i > 0; --i) {
			const node v = path[i];
			const int64_t prediction = 2*sign;

#if defined DEBUG
			assert(m_t.get_degree(v) == 2);
			assert(not has_valid_LV_prediction(v));
#endif

			m_predicted_LV[v] = prediction;
			m_predicted_LV__origin[v] = LV_propagation_origin::antenna_leaf;
			num_p2 += (prediction == +2);
			num_m2 += (prediction == -2);
			sign *= -1;
		}

		if (m_t.get_degree(h1) == 1) {
			const int64_t prediction = sign;

#if defined DEBUG
			assert(not has_valid_LV_prediction(h1));
#endif

			m_predicted_LV[h1] = prediction;
			m_predicted_LV__origin[h1] = LV_propagation_origin::antenna_leaf;
		}
	}

	m_path_info[path_idx].nodes_p2_to_assign = num_p2;
	m_path_info[path_idx].nodes_m2_to_assign = num_m2;

#if defined DEBUG
	assert(num_p2 + num_m2 + 2 == path.get_num_nodes());
#endif
}

void AEF_BnB::propagate_LV__antenna__from_internal(const node u) noexcept {
#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Found a trigger internal vertex for an antenna u= " << u << '\n';
	std::cout << tab() << "    Of level value= " << m_node_level[u] << '\n';
#endif

	m_predicted_LV[u] = m_node_level[u];
	m_predicted_LV__origin[u] = LV_propagation_origin::self;

	const int64_t sign_u = m_node_level[u] > 0 ? 1 : -1;

	const std::size_t path_u_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_u_idx];
	const std::size_t pu = path.get_position(u);

	const bool is_first_leaf = m_t.get_degree(path.get_h1()) == 1;
	const bool is_last_leaf = m_t.get_degree(path.get_h2()) == 1;

	uint64_t num_p2 = 0;
	uint64_t num_m2 = 0;

	num_p2 += (m_predicted_LV[u] == +2);
	num_m2 += (m_predicted_LV[u] == -2);

	int64_t sign;
	const std::size_t ini = not is_first_leaf;
	sign = (pu%2 == ini%2 ? sign_u : -sign_u);
	for (std::size_t i = ini; i < pu; ++i) {
		const node v = path[i];
		const int64_t prediction = sign*to_int64(m_t.get_degree(v));

#if defined DEBUG
		assert(m_t.get_degree(v) <= 2);
		assert(not has_valid_LV_prediction(v));
#endif

		m_predicted_LV[v] = prediction;
		m_predicted_LV__origin[v] = LV_propagation_origin::antenna_internal;
		num_p2 += (prediction == +2);
		num_m2 += (prediction == -2);
		sign *= -1;
	}

	const std::size_t fin = path.get_num_nodes() - (is_last_leaf ? 0 : 1);
	sign = -sign_u;
	for (std::size_t i = pu + 1; i < fin; ++i) {
		const node v = path[i];
		const int64_t prediction = sign*to_int64(m_t.get_degree(v));

#if defined DEBUG
		assert(m_t.get_degree(v) <= 2);
		assert(not has_valid_LV_prediction(v));
#endif

		m_predicted_LV[v] = prediction;
		m_predicted_LV__origin[v] = LV_propagation_origin::antenna_internal;
		num_p2 += (prediction == +2);
		num_m2 += (prediction == -2);
		sign *= -1;
	}

	m_path_info[path_u_idx].nodes_p2_to_assign = num_p2;
	m_path_info[path_u_idx].nodes_m2_to_assign = num_m2;

#if defined DEBUG
	assert(num_p2 + num_m2 + 2 == path.get_num_nodes());
#endif
}

void AEF_BnB::propagate_LV__antenna__from_hub
(const node h, const node u) noexcept
{
#if defined __LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Found a trigger hub for an antenna h= " << h << '\n';
	std::cout << tab() << "    Of level value= " << m_node_level[h] << '\n';
#endif

#if defined DEBUG
	assert(is_vertex_assigned(h));
	assert(not is_vertex_assigned(u));
#endif

	m_predicted_LV[h] = m_node_level[h];
	m_predicted_LV__origin[h] = LV_propagation_origin::self;

	m_predicted_LV[u] = -to_int64(m_t.get_degree(u));
	m_predicted_LV__origin[u] = LV_propagation_origin::antenna_hub;

	const std::size_t path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];
	const std::size_t pu = path.get_position(u);

	uint64_t num_p2 = 0;
	uint64_t num_m2 = 0;

	num_p2 += (m_predicted_LV[u] == +2);
	num_m2 += (m_predicted_LV[u] == -2);

	if (m_t.get_degree(path.get_h2()) == 1) {
#if defined DEBUG
		assert(pu == 1);
		assert(path[1] == u);
#endif

		int64_t sign = 1;
		for (std::size_t i = 2; i < path.get_num_nodes(); ++i) {
			const node v = path[i];
			const int64_t prediction = sign*to_int64(m_t.get_degree(v));

#if defined DEBUG
			assert(m_t.get_degree(v) <= 2);
			assert(not has_valid_LV_prediction(v));
#endif

			m_predicted_LV[v] = prediction;
			m_predicted_LV__origin[v] = LV_propagation_origin::antenna_hub;
			num_p2 += (prediction == +2);
			num_m2 += (prediction == -2);
			sign *= -1;
		}
	}
	else {
#if defined DEBUG
		assert(pu == path.get_num_nodes() - 2);
#endif

		int64_t sign = pu%2 == 0 ? -1 : 1;
		for (std::size_t i = 0; i < path.get_num_nodes() - 2; ++i) {
			const node v = path[i];
			const int64_t prediction = sign*to_int64(m_t.get_degree(v));

#if defined DEBUG
			assert(m_t.get_degree(v) <= 2);
			assert(not has_valid_LV_prediction(v));
#endif

			m_predicted_LV[v] = prediction;
			m_predicted_LV__origin[v] = LV_propagation_origin::antenna_hub;
			num_p2 += (prediction == +2);
			num_m2 += (prediction == -2);
			sign *= -1;
		}
	}

	m_path_info[path_idx].nodes_p2_to_assign = num_p2;
	m_path_info[path_idx].nodes_m2_to_assign = num_m2;

#if defined DEBUG
	assert(num_p2 + num_m2 + 2 == path.get_num_nodes());
#endif
}

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
