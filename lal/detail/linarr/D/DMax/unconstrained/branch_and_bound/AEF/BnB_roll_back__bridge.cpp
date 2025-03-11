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

void AEF_BnB::roll_back_LV_bridge_from_hub_h2(const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const std::size_t N = path.get_num_nodes();

	const auto stop = [&](const std::size_t k) -> bool
	{
		const node v = path[k];
		return m_predicted_LV_origin[v] != LV_propagation_origin::bridge_hub_2;
	};

	std::size_t i = N - 2;
	while (i > 0 and not stop(i)) {
		const node v = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
#endif
		m_predicted_LV_origin[v] = LV_propagation_origin::none;
		--i;
	}
}
void AEF_BnB::roll_back_LV_bridge_from_hub_h1(const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const std::size_t N = path.get_num_nodes();

	const auto stop = [&](const std::size_t k) -> bool
	{
		const node v = path[k];
		return m_predicted_LV_origin[v] != LV_propagation_origin::bridge_hub_1;
	};

	std::size_t i = 1;
	while (i < N - 1 and not stop(i)) {
		const node v = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
#endif
		m_predicted_LV_origin[v] = LV_propagation_origin::none;
		++i;
	}
}
void AEF_BnB::roll_back_LV_bridge_from_hub(
	const node h, const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Roll back through a bridge from only one hub...\n";
	std::cout << tab() << "    hub= " << h << '\n';
	std::cout << tab() << "    is h1? " << std::boolalpha
			  << (h == path.get_h1()) << '\n';
	std::cout << tab() << "    is h2? " << std::boolalpha
			  << (h == path.get_h2()) << '\n';
#endif

	if (h == path.get_h1()) {
		roll_back_LV_bridge_from_hub_h1(path_idx);
	}
	else {
		roll_back_LV_bridge_from_hub_h2(path_idx);
	}
}

void AEF_BnB::roll_back_LV_bridge_from_lowest_level_0_towards_h2(
	const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
	const std::size_t N = path.get_num_nodes();

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "    Roll back towards h2...\n";
#endif

	for (std::size_t i = pw + 1; i < N - 1; ++i) {
		const node v = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
		assert(
			m_predicted_LV_origin[v] == LV_propagation_origin::bridge_lowest_0
		);
#endif
		m_predicted_LV_origin[v] = LV_propagation_origin::none;
	}
}
void AEF_BnB::roll_back_LV_bridge_from_lowest_level_0_towards_h1(
	const std::size_t path_idx
) noexcept
{
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "    Roll back towards h1...\n";
#endif

	for (std::size_t i = pw - 1; i > 0; --i) {
		const node v = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
		assert(
			m_predicted_LV_origin[v] == LV_propagation_origin::bridge_lowest_0
		);
#endif
		m_predicted_LV_origin[v] = LV_propagation_origin::none;
	}
}
void AEF_BnB::roll_back_LV_bridge_from_lowest_level_0(const node u) noexcept
{
	const std::size_t path_idx = m_node_to_path_idx[u];
	const auto& path = m_paths_in_tree[path_idx];
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Roll back through a bridge from lowest 0...\n";
	std::cout << tab() << "    w= " << w << '\n';
#endif

#if defined DEBUG
	assert(w == u);
#endif

	m_predicted_LV_origin[w] = LV_propagation_origin::none;

	if (path.get_num_nodes() == 3) {
		return;
	}

	const node wm1 = path[pw - 1];
	if (has_valid_LV_prediction(wm1) and
		m_predicted_LV_origin[wm1] == LV_propagation_origin::bridge_lowest_0) {
		roll_back_LV_bridge_from_lowest_level_0_towards_h1(path_idx);
	}

	const node wp1 = path[pw + 1];
	if (has_valid_LV_prediction(wp1) and
		m_predicted_LV_origin[wp1] == LV_propagation_origin::bridge_lowest_0) {
		roll_back_LV_bridge_from_lowest_level_0_towards_h2(path_idx);
	}
}

void AEF_BnB::roll_back_LV_bridge_from_lowest_level_pm2(const node u) noexcept
{
	const auto& path = m_paths_in_tree[m_node_to_path_idx[u]];
	const node w = path.get_lowest_lexicographic();
	const std::size_t N = path.get_num_nodes();

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Roll back through a bridge from lowest +-2...\n";
	std::cout << tab() << "    u= " << u << '\n';
	std::cout << tab() << "    w= " << w << '\n';
#endif

	const auto stop = [&](const std::size_t k) -> bool
	{
		const node v = path[k];
		return m_predicted_LV_origin[v] !=
			   LV_propagation_origin::bridge_lowest_pm2;
	};

	m_predicted_LV_origin[w] = LV_propagation_origin::none;

	std::size_t p = path.get_position(w) - 1;
	while (p > 0 and not stop(p)) {
		const node v = path[p];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
#endif

		m_predicted_LV_origin[v] = LV_propagation_origin::none;
		--p;
	}

	p = path.get_position(w) + 1;
	while (p < N - 1 and not stop(p)) {
		const node v = path[p];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
#endif

		m_predicted_LV_origin[v] = LV_propagation_origin::none;
		++p;
	}
}

void AEF_BnB::roll_back_LV_bridge_from_internal(const node u) noexcept
{
	const auto& path = m_paths_in_tree[m_node_to_path_idx[u]];
	const std::size_t N = path.get_num_nodes();
	const node w = path.get_lowest_lexicographic();
	const std::size_t pw = path.get_position(w);
	const std::size_t pu = path.get_position(u);

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "Roll back through a bridge from internal...\n";
	std::cout << tab() << "    u= " << u << '\n';
	std::cout << tab() << "    w= " << w << '\n';
#endif

#if defined DEBUG
	assert(has_valid_LV_prediction(u));
	assert(m_node_level[u] == 2);
	assert(u != w);
#endif

	const LV_propagation_origin orig = [&]()
	{
		return pu < pw ? LV_propagation_origin::bridge_internal_left
					   : LV_propagation_origin::bridge_internal_right;
	}();

#if defined LAL_DEBUG_DMax_Unc_BnB
	std::cout << tab() << "    Origin detected: "
			  << LV_propagation_origin_to_short_string(orig) << " -- "
			  << LV_propagation_origin_to_string(orig) << '\n';
#endif

	m_predicted_LV_origin[u] = LV_propagation_origin::none;

	std::size_t i;

	i = pu + 1;
	while (i < N - 1 and path[i] != w) {
		const node v = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
#endif
		m_predicted_LV_origin[v] = LV_propagation_origin::none;
		++i;
	}

	i = pu - 1;
	while (i > 0 and path[i] != w) {
		const node v = path[i];
#if defined DEBUG
		assert(has_valid_LV_prediction(v));
#endif
		m_predicted_LV_origin[v] = LV_propagation_origin::none;
		--i;
	}

	if (m_predicted_LV_origin[w] == orig) {
		m_predicted_LV_origin[w] = LV_propagation_origin::none;
	}
}

} // namespace unconstrained
} // namespace DMax
} // namespace detail
} // namespace lal
