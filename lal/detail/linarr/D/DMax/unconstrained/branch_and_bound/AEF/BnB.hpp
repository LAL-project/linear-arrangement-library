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

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
#include <string_view>
#endif


// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/properties/branchless_path.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/detail/data_array.hpp>
#include <lal/detail/linear_set.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/sorting/counting_sort.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/maximum_arrangements.hpp>

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
#if not defined DEBUG
#error("'__LAL_PRINT_MESSAGES_DMax_Unc_BnB' must be defined along with 'DEBUG'")
#endif
#endif

namespace lal {
namespace detail {
namespace DMax {
namespace unconstrained {

/**
 * @brief A Branch and Bound algorithm for the maximum sum of edge lengths.
 *
 * Readers are urged to read \cite Alemany2023a for the full details of the
 * inner workings of the algorithm.
 *
 * In many of the parameters of the functions of this class we find the names:
 * - D_p: this is the sum of edge lengths of the edges contained entirely in
 * the prefix of the arrangement.
 * - D_ps_m: this is the sum of edge lengths of the parts over the prefix of the
 * arrangement of the edges partially contained in the prefix,
 */
class AEF_BnB {
public:
	static constexpr char VERTEX_ASSIGNED = 1;
	static constexpr char VERTEX_UNASSIGNED = 0;

	static constexpr bool debug_Unc_BnB_01 =
#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	true;
#else
	false;
#endif

	typedef std::conditional_t<debug_Unc_BnB_01, bool, void>
	exe_result_type;

public:
	// enumeration used in the function @ref process_end()/2
	// These values are used as flags!
	enum process_end_result {
		// algorithm did not reach the end of the arrangement
		did_not_reach_end	= 0b00000000,
		// algorithm reached the end of the arrangement
		reached_end			= 0b00000010,
		// algorithm reached found a new maximum
		found_max			= 0b00000100
	};

	[[nodiscard]]
	constexpr bool did_reach_end(const int at) const noexcept
	{ return at & process_end_result::reached_end; }

	[[nodiscard]]
	constexpr bool did_find_max(const int at) const noexcept
	{ return at & process_end_result::found_max; }

	enum class next_action {
		bound,
		continue_normally,
		continue_independent_set,
		continue_independent_set_leaves
	};

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	[[nodiscard]] constexpr std::string_view next_action_type_to_string
	(const next_action at) const noexcept {
		switch (at) {
		case next_action::bound:
			return "Bound computation";
		case next_action::continue_normally:
			return "Continue computation";
		case next_action::continue_independent_set:
			return "Continue computation for an independent set";
		case next_action::continue_independent_set_leaves:
			return "Continue computation for an independent set of leaves";
		}
		return "";
	}
#endif

public:
	/// Reference to the input free tree
	const graphs::free_tree& m_t;
	/// Temporary memory to store @ref m_t as a rooted tree.
	graphs::rooted_tree m_rt;

	/**
	 * @brief Complete result of the algorithm
	 *
	 * The entire set of maximum arrangements we can make starting at the given
	 * vertex.
	 */
	set_max_arrangements m_max_arrs;
	/**
	 * @brief Partial result of the algorithm.
	 *
	 * This arrangement is built from left to right, adding vertices one by one.
	 */
	linear_arrangement m_arr;

public:
	/**
	 * @brief Constructor
	 * @param t Input tree.
	 * @param leaves Set of leaves per vertex.
	 * @param colors Bipartite coloring of the tree.
	 * @param num_verts_blue Number of blue vertices.
	 * @param num_verts_red Number of red vertices.
	 * @param paths_in_tree All the branchless paths in the tree.
	 * @param node_to_path_idx An index for each vertex pointing to the branchless
	 * path it belongs.
	 * @param orbits The set of vertex orbits of the tree.
	 * @param vertex_to_orbit An index for each vertex pointing to the vertex
	 * orbit it belongs.
	 */
	AEF_BnB(
		const graphs::free_tree& t,
		const data_array<std::vector<node>>& leaves,
		// colors of vertices
		const properties::bipartite_graph_coloring& colors,
		const uint64_t num_verts_blue,
		const uint64_t num_verts_red,
		// paths
		const std::vector<properties::branchless_path>& paths_in_tree,
		const data_array<std::size_t>& node_to_path_idx,
		// orbits
		const std::vector<std::vector<node>>& orbits,
		const data_array<std::size_t>& vertex_to_orbit
	)
	noexcept;
	/// Destructor
	~AEF_BnB() noexcept = default;

	/// Initialize the Branch and Bound algorithm.
	void initialise
	(const std::pair<uint64_t, linear_arrangement>& initial_DMax)
	noexcept;

	/// Execute the algorithm starting at the given vertex
	void exe(node first_node) noexcept;

protected:

	// -- constraints --

	/**
	 * @brief Function to discard a vertex as the next vertex to add to the arrangement.
	 *
	 * This function implements symmetry breaking constraints, optimality
	 * constraints, ... For details see code, or the paper \cite Alemany2023a.
	 * @param u Vertex to test.
	 * @param pos Position where the vertex is to be placed at.
	 * @returns Whether or not the vertex is to be discarded at this stage.
	 */
	[[nodiscard]] bool discard_vertex(const node u, const position_t pos)
	const noexcept;

	// -- bounds --

	/// Calculate a 'generic' upper bound
	[[nodiscard]] uint64_t upper_bound_generic
	(const uint64_t D_p, const uint64_t D_ps_m, const position_t pos) noexcept;

	/// Decide what to do next according to the value of the upper bound.
	[[nodiscard]] next_action what_to_do_next
	(const uint64_t D_p, const uint64_t D_ps_m, const position_t pos) noexcept;

	// -- state manipulation --

	/**
	 * @brief Update the internal state of the algorithm.
	 *
	 * Assuming that vertex @e u will be placed at position @e pos.
	 * @param u Next vertex in the arrangement.
	 * @param pos Next position.
	 * @param D_p Sum of edge lengths of edges in the prefix.
	 * @param D_ps_m Sum of edge lengths of edges partially in the prefix.
	 */
	void update_state
	(const node u, const position_t pos, uint64_t& D_p, uint64_t& D_ps_m)
	noexcept;

	/**
	 * @brief Update the internal state of the algorithm.
	 *
	 * Assuming that the vertex at position @e pos will be removed.
	 * @param pos Position to remove the vertex from.
	 */
	void recover_state(const position_t pos) noexcept;

	/**
	 * @brief Take action at the end of the recursion.
	 *
	 * The algorithm has finished the construction of a linear arrangement and
	 * it's time to add it to the set of maximum arrangements.
	 * @param D Cost of the arrangement.
	 * @param pos Position to place the next vertex (it equals the number of
	 * vertices in this function).
	 * @returns Whether the recursion actually constructed the arrangement.
	 * The values returned are a combination of the values in @ref process_end_result.
	 */
	[[nodiscard]] int process_end(const uint64_t D, const position pos) noexcept;

	// -- others --

	/// Is vertex @e u assigned to the prefix of the arrangement?
	[[nodiscard]] bool is_vertex_assigned(node u) const noexcept
	{ return m_is_node_assigned[u] == VERTEX_ASSIGNED; }

	/// Is vertex @e u a thistle vertex?
	[[nodiscard]] bool is_vertex_thistle(node u) const noexcept {
#if defined DEBUG
		assert(is_vertex_assigned(u));
#endif
		return to_uint64(std::abs(m_node_level[u])) != m_t.get_degree(u);
	}

	/// Return the only parent of the leaf @e u.
	[[nodiscard]] node leaf_parent(node u) const noexcept {
#if defined DEBUG
		assert(m_t.get_degree(u) == 1);
#endif
		return m_t.get_neighbours(u)[0];
	}

	// -------------------------------------------------------------------------
	// execute Branch and Bound for an independent set of vertices

	/* Functions
	 *		'exe_independent_set'
	 * and
	 *		'exe_independent_set_leaves'
	 * do not call the 'update_state' functions since the construction of the
	 * arrangement is easy, final, and the information updated by the 'update_state'
	 * function is not needed anymore. Moreover, each call to 'update_state' requires
	 * a subsequent call to 'recover_state'.
	 *
	 * The value of D passed as parameter, 'D_p' is the sum of the lengths of all
	 * the edges whose both endpoints are assigned to the prefix of the arrangement
	 * before calling the function.
	 */

	/**
	 * @brief Finish constructing the arrangement.
	 *
	 * Under the assumption that the unassigned vertices make up an independent
	 * set.
	 * @param D_p Sum of the lengths of the edges contained inside the prefix.
	 * @param pos Position where to continue building the arrangement from.
	 * @returns Whether or not the algorithm found a new maximum arrangement.
	 */
	exe_result_type exe_independent_set
	(const uint64_t D_p, position pos) noexcept;

	/**
	 * @brief Finish constructing the arrangement.
	 *
	 * Under the assumption that the unassigned vertices make up an independent
	 * set, and that all remaining vertices are leaves.
	 * @param D_p Sum of the lengths of the edges contained inside the prefix.
	 * @param pos Position where to continue building the arrangement from.
	 * @returns Whether or not the algorithm found a new maximum arrangement.
	 */
	exe_result_type exe_independent_set_leaves
	(const uint64_t D_p, position pos) noexcept;

	// -------------------------------------------------------------------------
	// execute Branch and Bound for any tree

	/** @brief Execute the Branch and Bound algorithm.
	 *
	 * The algorithm will add vertices starting at position 'pos'.
	 *
	 * @param D_p The sum of the lengths of the edges in E_p, that is, the set of
	 * edges where both endpoints are assigned to the prefix.
	 * @param D_ps_m The sum of the partial length of the edges in E_ps, from the
	 * vertex assigned to the prefix to the border that separates the prefix and
	 * the suffix (between positions 'pos-1' and 'pos'), calculated as the length
	 * of the edge as if it ended at position 'pos'.
	 * @param pos Position where to place the next vertex.
	 */
	exe_result_type exe
	(const uint64_t D_p, const uint64_t D_ps_m, const position pos)
	noexcept;

private:
	// -------------------------------------------------------------------------
	// Tree-related data

	/// Number of nodes.
	const uint64_t m_n_nodes;
	/// Set of leaves per vertex.
	const data_array<std::vector<node>>& m_leaves;
	/// Bipartite coloring of the tree.
	const properties::bipartite_graph_coloring& m_vertex_colors;
	/// Number of blue vertices (@ref m_vertex_colors).
	const uint64_t m_num_verts_blue;
	/// Number of red vertices (@ref m_vertex_colors).
	const uint64_t m_num_verts_red;
	/// Number of blue vertices that are assigned to the prefix of the arrangement (@ref m_vertex_colors).
	uint64_t m_num_assigned_verts_blue;
	/// Number of red vertices that are assigned to the prefix of the arrangement (@ref m_vertex_colors).
	uint64_t m_num_assigned_verts_red;
	/// All the branchless paths of the tree.
	const std::vector<properties::branchless_path>& m_paths_in_tree;
	/// An index for each vertex that points to its path in @ref m_paths_in_tree.
	const data_array<std::size_t>& m_node_to_path_idx;
	/// The number of thistle vertices for each branchless path.
	data_array<uint8_t> m_num_thistle_per_path;
	/// The set of vertex orbits of the tree.
	const std::vector<std::vector<node>>& m_orbits;
	/// An index for each vertex that points to its vertex orbit in @ref m_orbits.
	const data_array<std::size_t>& m_vertex_to_orbit;

	// -------------------------------------------------------------------------
	// Data used for upper bounds

	/// Frequency of degrees among unassigned vertices.
	data_array<uint64_t> m_degree_count;

	/// Number of assigned neighbor vertices for each vertex.
	data_array<uint64_t> m_num_assigned_neighbors;
	/// Number of unassigned neighbor vertices for each vertex.
	data_array<uint64_t> m_num_unassigned_neighbors;

	/* -------------------------- BORDER VERTICES -------------------------- */
	// memory used to sort those vertices with some neighbor assigned.
	// sorted by amount of assigned neighbors

	/// The set of border vertices.
	linear_set<node> m_border_vertices;

	/* -------------------------- BORDER VERTICES -------------------------- */

	// -------------------------------------------------------------------------
	// Algorithm control

	/// First vertex with which to start the algorithm.
	node m_first_vertex;

	/// The set of assigned nodes.
	data_array<char> m_is_node_assigned;

	/// A helper class to be able to store edges in @ref m_E_p, @ref m_E_ps, @ref m_E_s.
	class indexer_edge {
	private:
		std::size_t m_capacity;

	public:
		void init(std::size_t cap) noexcept {
			m_capacity = cap;
		}
		[[nodiscard]] std::size_t operator()(const edge& p) const noexcept {
			return p.first + p.second*m_capacity;
		}
	};

	/// The set of edges fully contained in the prefix.
	linear_set<edge, indexer_edge> m_E_p;
	/// The set of edges with one endpoint in the prefix, the other in the suffix.
	linear_set<edge, indexer_edge> m_E_ps;
	/// The set of edges fully contained in the suffix.
	linear_set<edge, indexer_edge> m_E_s;

	/// Directional left degree of each vertex.
	data_array<uint64_t> m_node_left_degree;
	/// Directional right degree of each vertex.
	data_array<uint64_t> m_node_right_degree;
	/// The level value of each vertex.
	data_array<int64_t> m_node_level;

	/// Cuts in the arrangement
	data_array<uint64_t> m_cut_values;

#if defined __LAL_PRINT_MESSAGES_DMax_Unc_BnB
	// -------------------------------------------------------------------------
	// Display of algorithm's data and progress

	void output_arrangement() const noexcept;
	void output_invarr(position p) const noexcept;
	void output_degree_sequence(position p) const noexcept;
	void output_left_degree_sequence(position p) const noexcept;
	void output_right_degree_sequence(position p) const noexcept;
	void output_level_sequence(position p) const noexcept;
	void output_cut_signature(position p) const noexcept;
	void output_num_assigned_neighbors() const noexcept;
	void output_num_unassigned_neighbors() const noexcept;
	void output_border_vertices() const noexcept;
	void display_all_info
	(const uint64_t D_p, const uint64_t D_ps_m, const position pos)
	noexcept;

	// progress of algorithm
	std::string m_tabstr;
	const std::string& tab() const noexcept { return m_tabstr; }
	void push_tab(const std::string& add = "|   ") noexcept { m_tabstr += add; }
	void pop_tab() noexcept { m_tabstr = m_tabstr.substr(0, m_tabstr.length() - 4); }
#endif
};

} // -- namespace unconstrained
} // -- namespace DMax
} // -- namespace detail
} // -- namespace lal
