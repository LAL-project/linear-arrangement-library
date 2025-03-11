/*********************************************************************
 *
 * Linear Arrangement Library - A library that implements a collection
 * algorithms for linear arrangments of graphs.
 *
 * Copyright (C) 2019 - 2025
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
#if defined LAL_DEBUG_DMax_Unc_BnB
#include <iostream>
#endif
#endif

// lal includes
#include <lal/detail/macros/basic_convert.hpp>
#if defined LAL_DEBUG_DMax_Unc_BnB
#include <lal/graphs/output.hpp>
#endif

// DMax includes
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/BnB.hpp>

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

propagation_result AEF_BnB::propagate_LV_bridge_check_lowest_can_be_predicted(
	const std::size_t path_idx, const LV_propagation_origin origin
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);

	const node wm1 = path[pw - 1];
	const node wp1 = path[pw + 1];
	if (not(has_valid_LV_prediction(wm1) and has_valid_LV_prediction(wp1))) {
		return propagation_result::success;
	}

	const std::size_t N = path.get_num_nodes();
	const int64_t level_m1 = m_predicted_LV[wm1];
	const int64_t level_p1 = m_predicted_LV[wp1];
	int64_t prediction_LV_w;

#if defined DEBUG
	const node h1 = path.get_h1();
	const node h2 = path.get_h2();
#endif

	if (N == 3) {
		prediction_LV_w = -2;
	}
	else if (path[1] == w) {
#if defined DEBUG
		assert(wm1 == h1);
#endif
		// left degree: +1 due to h1
		if (level_p1 == 2) {
			// left degree: +1 due to wp1
			prediction_LV_w = -2;
		}
		else {
			// right degree: +1 due to wp1
			prediction_LV_w = 0;
		}
	}
	else if (path[N - 2] == w) {
#if defined DEBUG
		assert(wp1 == h2);
#endif
		// left degree: +1 due to h2
		if (level_m1 == 2) {
			// left degree: +1 due to wm1
			prediction_LV_w = -2;
		}
		else {
			// right degree: +1 due to wm1
			prediction_LV_w = 0;
		}
	}
	else {
		if (level_m1 == level_p1) {
			// either +2 or -2
			prediction_LV_w = -level_p1;
		}
		else {
			prediction_LV_w = 0;
		}
	}

	if (not has_valid_LV_prediction(w)) {
		m_predicted_LV[w] = prediction_LV_w;
		m_predicted_LV_origin[w] = origin;
		return propagation_result::success;
	}

	return m_predicted_LV[w] == prediction_LV_w
			   ? propagation_result::success
			   : propagation_result::conflict_LV_propagation;
}

void AEF_BnB::propagate_LV_bridge_from_hub_h2(const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t N = path.get_num_nodes();

	const auto stop = [&](const std::size_t k) -> bool
	{
		const node v = path[k];
		return is_vertex_assigned(v) or has_valid_LV_prediction(v) or v == w;
	};

	int64_t sign = -1;
	std::size_t i = N - 2;
	while (i > 0 and not stop(i)) {
		const node v = path[i];
#if defined DEBUG
		assert(not has_valid_LV_prediction(v));
		assert(v != w);
#endif
		m_predicted_LV[v] = 2 * sign;
		m_predicted_LV_origin[v] = LV_propagation_origin::bridge_hub_2;
		sign = -sign;
		--i;
	}
}
void AEF_BnB::propagate_LV_bridge_from_hub_h1(const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t N = path.get_num_nodes();

	const auto stop = [&](const std::size_t k) -> bool
	{
		const node v = path[k];
		return is_vertex_assigned(v) or has_valid_LV_prediction(v) or v == w;
	};

	int64_t sign = -1;
	std::size_t i = 1;
	while (i < N - 1 and not stop(i)) {
		const node v = path[i];
#if defined DEBUG
		assert(not has_valid_LV_prediction(v));
		assert(v != w);
#endif
		m_predicted_LV[v] = 2 * sign;
		m_predicted_LV_origin[v] = LV_propagation_origin::bridge_hub_1;
		sign = -sign;
		++i;
	}
}
propagation_result AEF_BnB::propagate_LV_bridge_from_hub(
	const node h, const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
#if defined DEBUG
	const node w = path.get_lowest_lexicographic();
#endif

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab()
			  << "Propagate through a bridge from one of the hubs...\n";
	std::cout << tab() << "    hub= " << h << '\n';
	std::cout << tab() << "    is h1? " << std::boolalpha
			  << (h == path.get_h1()) << '\n';
	std::cout << tab() << "    is h2? " << std::boolalpha
			  << (h == path.get_h2()) << '\n';
#endif

	m_predicted_LV[h] = m_node_level[h];
	m_predicted_LV_origin[h] = LV_propagation_origin::self;

	LV_propagation_origin origin;
	if (h == path.get_h1()) {
		origin = LV_propagation_origin::bridge_hub_1;
		propagate_LV_bridge_from_hub_h1(path_idx);
#if defined DEBUG
		if (is_vertex_assigned(w) and m_node_level[w] == 0) {
			const std::size_t pw = path.get_position(w);
			const node wp1 = path[pw + 1];
			if (wp1 != path.get_h2()) {
				assert(has_valid_LV_prediction(wp1));
			}
		}
#endif
	}
	else {
		origin = LV_propagation_origin::bridge_hub_2;
		propagate_LV_bridge_from_hub_h2(path_idx);
#if defined DEBUG
		if (is_vertex_assigned(w) and m_node_level[w] == 0) {
			const std::size_t pw = path.get_position(w);
			const node wm1 = path[pw - 1];
			if (wm1 != path.get_h1()) {
				assert(has_valid_LV_prediction(wm1));
			}
		}
#endif
	}

	return propagate_LV_bridge_check_lowest_can_be_predicted(
		path_idx, origin
	);
}

void AEF_BnB::propagate_LV_bridge_from_lowest_level_0_towards_h2(
	const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
	const std::size_t N = path.get_num_nodes();
	const node wm1 = path[pw - 1];

	int64_t sign = m_predicted_LV[wm1] == 2 ? -1 : 1;
	for (std::size_t i = pw + 1; i < N - 1; ++i) {
		const node v = path[i];
#if defined DEBUG
		assert(not has_valid_LV_prediction(v));
#endif
		m_predicted_LV[v] = 2 * sign;
		m_predicted_LV_origin[v] = LV_propagation_origin::bridge_lowest_0;
		sign = -sign;
	}
}
void AEF_BnB::propagate_LV_bridge_from_lowest_level_0_towards_h1(
	const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
	const node wp1 = path[pw + 1];

	int64_t sign = m_predicted_LV[wp1] == 2 ? -1 : 1;
	for (std::size_t i = pw - 1; i > 0; --i) {
		const node v = path[i];
#if defined DEBUG
		assert(not has_valid_LV_prediction(v));
#endif
		m_predicted_LV[v] = 2 * sign;
		m_predicted_LV_origin[v] = LV_propagation_origin::bridge_lowest_0;
		sign = -sign;
	}
}
void AEF_BnB::propagate_LV_bridge_from_lowest_level_0(const node u) noexcept
{
	const std::size_t path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Propagate through a bridge from lowest 0...\n";
	std::cout << tab() << "    w= " << w << '\n';
#endif

#if defined DEBUG
	assert(w == u);
	assert(m_node_level[w] == 0);
#endif

	m_predicted_LV[w] = 0;
	m_predicted_LV_origin[w] = LV_propagation_origin::self;

	if (path.get_num_nodes() == 3) {
		return;
	}

	const node wm1 = path[pw - 1];
	const node wp1 = path[pw + 1];

#if defined DEBUG
	assert(has_valid_LV_prediction(wm1) or has_valid_LV_prediction(wp1));
#endif

	if (has_valid_LV_prediction(wm1)) {
		propagate_LV_bridge_from_lowest_level_0_towards_h2(path_idx);
	}
	else if (has_valid_LV_prediction(wp1)) {
		propagate_LV_bridge_from_lowest_level_0_towards_h1(path_idx);
	}
}

propagation_result
AEF_BnB::propagate_LV_bridge_from_lowest_level_pm2(const node u) noexcept
{
	const auto& path = m_paths_in_tree[m_node_to_path_idx[u]];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
	const std::size_t N = path.get_num_nodes();

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Propagate through a bridge from lowest +-2...\n";
	std::cout << tab() << "    u= " << u << '\n';
	std::cout << tab() << "    w= " << w << '\n';
#endif

#if defined DEBUG
	assert(not has_valid_LV_prediction(w));
	assert(m_node_level[w] == 2);
	assert(u == w);
#endif

	m_predicted_LV[w] = m_node_level[w];
	m_predicted_LV_origin[w] = LV_propagation_origin::self;

	int64_t sign;
	std::size_t p;

	p = pw - 1;
	sign = -1;
	while (p > 0 and not has_valid_LV_prediction(path[p])) {
		const node v = path[p];
		const int64_t prediction = 2 * sign;
		m_predicted_LV[v] = prediction;
		m_predicted_LV_origin[v] = LV_propagation_origin::bridge_lowest_pm2;

		sign = -sign;
		--p;
	}
	{
		const node v = path[p];
		if (v != path.get_h1() and m_predicted_LV[v] != 2 * sign) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			std::cout << tab() << "Found a conflict in the propagation\n";
			std::cout << tab() << "    at vertex: " << v << '\n';
			std::cout << tab()
					  << "    Existing prediction= " << m_predicted_LV[v]
					  << '\n';
			std::cout << tab() << "    Current prediction= " << 2 * sign
					  << '\n';
#endif
			return propagation_result::conflict_LV_propagation;
		}
	}

	p = pw + 1;
	sign = -1;
	while (p < N - 1 and not has_valid_LV_prediction(path[p])) {
		const node v = path[p];
		const int64_t prediction = 2 * sign;
		m_predicted_LV[v] = prediction;
		m_predicted_LV_origin[v] = LV_propagation_origin::bridge_lowest_pm2;

		sign = -sign;
		++p;
	}
	{
		const node v = path[p];
		if (v != path.get_h2() and m_predicted_LV[v] != 2 * sign) {
#if defined LAL_DEBUG_DMax_Unc_BnB
			std::cout << tab() << "Found a conflict in the propagation\n";
			std::cout << tab() << "    at vertex: " << v << '\n';
			std::cout << tab()
					  << "    Existing prediction= " << m_predicted_LV[v]
					  << '\n';
			std::cout << tab() << "    Current prediction= " << 2 * sign
					  << '\n';
#endif
			return propagation_result::conflict_LV_propagation;
		}
	}

	return propagation_result::success;
}

propagation_result AEF_BnB::propagate_LV_bridge_from_internal(const node u
) noexcept
{
	const std::size_t path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];
	const std::size_t N = path.get_num_nodes();
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
	const std::size_t pu = path.get_position(u);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Propagate through a bridge from internal...\n";
	std::cout << tab() << "    u= " << u << '\n';
	std::cout << tab() << "    w= " << w << '\n';
#endif

#if defined DEBUG
	assert(not has_valid_LV_prediction(u));
	assert(m_node_level[u] == 2);
	assert(u != w);
#endif

	const LV_propagation_origin orig = [&]()
	{
		return pu < pw ? LV_propagation_origin::bridge_internal_left
					   : LV_propagation_origin::bridge_internal_right;
	}();

	m_predicted_LV[u] = m_node_level[u];
	m_predicted_LV_origin[u] = LV_propagation_origin::self;

	std::size_t i;
	int64_t sign;

	sign = -1;
	i = pu + 1;
	while (i < N - 1 and path[i] != w) {
		const node v = path[i];
#if defined DEBUG
		assert(not has_valid_LV_prediction(v));
#endif
		m_predicted_LV[v] = 2 * sign;
		m_predicted_LV_origin[v] = orig;
		sign = -sign;
		++i;
	}

	sign = -1;
	i = pu - 1;
	while (i > 0 and path[i] != w) {
		const node v = path[i];
#if defined DEBUG
		assert(not has_valid_LV_prediction(v));
#endif
		m_predicted_LV[v] = 2 * sign;
		m_predicted_LV_origin[v] = orig;
		sign = -sign;
		--i;
	}

	return propagate_LV_bridge_check_lowest_can_be_predicted(path_idx, orig);
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
