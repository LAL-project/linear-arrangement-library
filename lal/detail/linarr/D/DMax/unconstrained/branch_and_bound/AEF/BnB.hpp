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

#pragma once

// lal includes
#include <lal/basic_types.hpp>
#include <lal/graphs/free_tree.hpp>
#include <lal/graphs/rooted_tree.hpp>
#include <lal/properties/branchless_path.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/detail/array.hpp>
#include <lal/detail/set_array.hpp>
#include <lal/detail/macros/basic_convert.hpp>
#include <lal/detail/sorting/counting_sort.hpp>

#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/set_maximum_arrangements.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/reason_discard.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/next_action.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/level_value_propagation_origin.hpp>
#include <lal/detail/linarr/D/DMax/unconstrained/branch_and_bound/AEF/propagation_result.hpp>

#if defined __LAL_DEBUG_DMax_Unc_BnB
#if !defined DEBUG
#error("'__LAL_DEBUG_DMax_Unc_BnB' must be defined along with 'DEBUG'")
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
	/// Value to indicate that a vertex is assigned to the arrangement.
	static constexpr char VERTEX_ASSIGNED = 1;
	/// Value to indicate that a vertex is not assigned to the arrangement.
	static constexpr char VERTEX_UNASSIGNED = 0;

	/// Used to determine the result type of certain functions.
	static constexpr bool debug_BnB =
#if defined __LAL_DEBUG_DMax_Unc_BnB
	true;
#else
	false;
#endif

	/**
	 * @brief Result of the main function of the algorithm.
	 *
	 * If @ref debug_BnB is true, then the function is to return a Boolean value
	 * that indicates if it found a maxium arrangement.
	 */
	typedef std::conditional_t<debug_BnB, bool, void>
	exe_result_type;

public:
	/**
	 * @brief Enumeration used in the function @ref process_end
	 *
	 * These values are used as flags!
	 */
	enum process_end_result {
		/// Algorithm did not completely construct the arrangement.
		did_not_reach_end	= 0b00000000,
		/// Algorithm reached the end of the arrangement.
		reached_end			= 0b00000001,
		/// Algorithm reached found a new maximum.
		found_max			= 0b00000010
	};

	/// Returns true if @e at does not contain @ref process_end_result::did_not_reach_end.
	[[nodiscard]] constexpr bool did_reach_end(const int at) const noexcept
	{ return at & process_end_result::reached_end; }

	/// Returns true if @e at contains @ref process_end_result::found_max.
	[[nodiscard]] constexpr bool did_find_max(const int at) const noexcept
	{ return at & process_end_result::found_max; }

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
	set_maximum_arrangements m_max_arrs;
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
	 * @param incident_antennas For every vertex @e u, this contains the list of
	 * the first nodes in all incident antennas
	 * @param orbits The set of vertex orbits of the tree.
	 * @param vertex_to_orbit An index for each vertex pointing to the vertex
	 * orbit it belongs.
	 */
	AEF_BnB(
		const graphs::free_tree& t,
		const array<std::vector<node>>& leaves,
		// colors of vertices
		const properties::bipartite_graph_coloring& colors,
		const uint64_t num_verts_blue,
		const uint64_t num_verts_red,
		// paths
		const std::vector<properties::branchless_path>& paths_in_tree,
		const array<std::size_t>& node_to_path_idx,
		const array<std::vector<node>>& incident_antennas,
		// orbits
		const std::vector<std::vector<node>>& orbits,
		const array<std::size_t>& vertex_to_orbit
	)
	noexcept;
	/// Destructor
	~AEF_BnB() noexcept = default;

	/// Initialize the Branch and Bound algorithm.
	void initialize
	(const std::pair<uint64_t, linear_arrangement>& initial_DMax)
	noexcept;

	/// Execute the algorithm starting at the given vertex
	void exe(node first_node) noexcept;

protected:

	// -- constraints --

	/**
	 * @brief Can the propagation from vertex @e u to vertex @e v fail?
	 *
	 * This is only checked for pairs of vertices connected by a propagation path
	 * (any pair of vertices in an antenna, or pairs of vertices in a bridge
	 * where both are on the same side of its lowest lexicographic).
	 */
	[[nodiscard]] reason_discard check_propagation_node_to_node(
		const node u, const int64_t level_u,
		const node v, const int64_t level_v
	) const noexcept;
	/// Can a vertex of a bridge be discarded when it is to be assigned with level 0?
	[[nodiscard]] reason_discard discard_node__degree_2__bridge__level_0
	(const node u)
	const noexcept;
	/// Can a vertex of a bridge be discarded when it is to be assigned with level \f$\pm2\f$?
	[[nodiscard]] reason_discard discard_node__degree_2__bridge__level_pm2
	(const node u, const int64_t level_u)
	const noexcept;
	/// Can a vertex of degree 2 be discarded?
	[[nodiscard]] reason_discard discard_node_degree_2
	(const node u, const int64_t level_u)
	const noexcept;
	/// Can a vertex of degree 3 be discarded?
	[[nodiscard]] reason_discard discard_node_degree_3
	(const node u, const int64_t level_u)
	const noexcept;

	/**
	 * @brief Function to discard a vertex as the next vertex to add to the arrangement.
	 *
	 * This function implements symmetry breaking constraints, optimality
	 * constraints, ... For details see code, or the paper \cite Alemany2023a.
	 * @param u Vertex to test.
	 * @param pos Position where the vertex is to be placed at.
	 * @returns Whether or not the vertex is to be discarded at this stage.
	 */
	[[nodiscard]] reason_discard discard_vertex(const node u, const position_t pos)
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

	// -- propagation of constraints -- //

	/// Propagate level values at an antenna, starting at its hub.
	void propagate_LV__antenna__from_hub
	(const node h, const node u) noexcept;
	/// Propagate level values at an antenna, starting at its leaf.
	void propagate_LV__antenna__from_leaf
	(const node u) noexcept;
	/// Propagate level values at an antenna, starting at an internal vertex.
	void propagate_LV__antenna__from_internal
	(const node u) noexcept;

	/**
	 * @brief In a bridge, predict the level value of the lowest lexicographic vertex.
	 *
	 * Checks if, after several propagations on the bridge, the level value of the
	 * lowest lexicographic vertex can be predicted, and assigns one when appropriate.
	 * This check can fail.
	 */
	[[nodiscard]] propagation_result propagate_LV__bridge__check_lowest_can_be_predicted
	(const std::size_t path_idx, const LV_propagation_origin origin)
	noexcept;

	/// Propagate level values at a bridge, starting at the second hub.
	void propagate_LV__bridge__from_hub__h2
	(const std::size_t path_idx) noexcept;
	/// Propagate level values at a bridge, starting at the first hub.
	void propagate_LV__bridge__from_hub__h1
	(const std::size_t path_idx) noexcept;
	/// Propagate level values at a bridge, starting at a hub @e h.
	[[nodiscard]] propagation_result propagate_LV__bridge__from_hub
	(const node h, const std::size_t path_idx) noexcept;

	/// Propagate level values at a bridge starting at the lowest lexicographic
	/// with level value \f$\pm 2\f$ towards the second hub.
	void propagate_LV__bridge__from_lowest__level_0__towards_h2
	(const std::size_t path_idx) noexcept;
	/// Propagate level values at a bridge starting at the lowest lexicographic
	/// with level value \f$\pm 2\f$ towards the first hub.
	void propagate_LV__bridge__from_lowest__level_0__towards_h1
	(const std::size_t path_idx) noexcept;
	/// Propagate level values at a bridge starting at the lowest lexicographic.
	void propagate_LV__bridge__from_lowest__level_0
	(const node u) noexcept;

	/// Propagate level values at a bridge starting at the lowest lexicographic
	/// with level value \f$0\f$.
	[[nodiscard]] propagation_result propagate_LV__bridge__from_lowest__level_pm2
	(const node u) noexcept;

	/// Propagate level values at a bridge starting at an internal vertex that is
	/// not the lowest lexicographic.
	[[nodiscard]] propagation_result propagate_LV__bridge__from_internal
	(const node u) noexcept;

	/// Propagate level values starting at vertex @e u.
	[[nodiscard]] propagation_result propagate_constraints
	(const node u) noexcept;

	// -- rollback constraints -- //

	/// Undo the propagation of level values at an antenna.
	void roll_back_LV__antenna
	(const node u) noexcept;

	/// Undo the propagation of level values at a bridge, starting at the second hub.
	void roll_back_LV__bridge__from_hub__h2
	(const std::size_t path_idx) noexcept;
	/// Undo the propagation of level values at a bridge, starting at the first hub.
	void roll_back_LV__bridge__from_hub__h1
	(const std::size_t path_idx) noexcept;
	/// Undo the propagation of level values at a bridge.
	void roll_back_LV__bridge__from_hub
	(const node h, const std::size_t path_idx) noexcept;

	/// Undo the propagation of level values at a bridge, starting at the lowest
	/// lexicographic of level value \f$0\f$ towards the second hub.
	void roll_back_LV__bridge__from_lowest__level_0__towards_h2
	(const std::size_t path_idx) noexcept;
	/// Undo the propagation of level values at a bridge, starting at the lowest
	/// lexicographic of level value \f$0\f$ towards the first hub.
	void roll_back_LV__bridge__from_lowest__level_0__towards_h1
	(const std::size_t path_idx) noexcept;
	/// Undo the propagation of level values at a bridge, starting at the lowest
	/// lexicographic of level value \f$0\f$.
	void roll_back_LV__bridge__from_lowest__level_0
	(const node u) noexcept;

	/// Undo the propagation of level values at a bridge, starting at the lowest
	/// lexicographic of level value \f$\pm2\f$.
	void roll_back_LV__bridge__from_lowest__level_pm2
	(const node u) noexcept;

	/// Undo the propagation of level values at a bridge, starting at an internal
	/// vertex.
	void roll_back_LV__bridge__from_internal
	(const node u) noexcept;
	/// Undo the propagation of level values at a vertex @e u.
	void roll_back_constraints
	(const node u) noexcept;

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
	[[nodiscard]] bool is_vertex_assigned(const node u) const noexcept {
		return m_is_node_assigned[u] == VERTEX_ASSIGNED;
	}

	/// Is vertex @e u a thistle vertex?
	[[nodiscard]] bool is_vertex_thistle(const node u) const noexcept {
#if defined DEBUG
		assert(is_vertex_assigned(u));
#endif
		return to_uint64(std::abs(m_node_level[u])) != m_t.get_degree(u);
	}

	/// Return the only parent of the leaf @e u.
	[[nodiscard]] node leaf_parent(const node u) const noexcept {
#if defined DEBUG
		assert(m_t.get_degree(u) == 1);
#endif
		return m_t.get_neighbors(u)[0];
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
	const array<std::vector<node>>& m_leaves;
	/// Bipartite coloring of the tree.
	const properties::bipartite_graph_coloring& m_vertex_colors;
	/// Number of blue vertices (@ref m_vertex_colors).
	const uint64_t m_num_nodes_blue;
	/// Number of red vertices (@ref m_vertex_colors).
	const uint64_t m_num_nodes_red;
	/// Number of blue vertices that are assigned to the prefix of the arrangement (@ref m_vertex_colors).
	uint64_t m_num_assigned_nodes_blue;
	/// Number of red vertices that are assigned to the prefix of the arrangement (@ref m_vertex_colors).
	uint64_t m_num_assigned_nodes_red;
	/// All the branchless paths of the tree.
	const std::vector<properties::branchless_path>& m_paths_in_tree;
	/// An index for each vertex that points to its path in @ref m_paths_in_tree.
	const array<std::size_t>& m_node_to_path_idx;
	const array<std::vector<node>>& m_incident_antennas;
	/// The set of vertex orbits of the tree.
	const std::vector<std::vector<node>>& m_orbits;
	/// An index for each vertex that points to its vertex orbit in @ref m_orbits.
	const array<std::size_t>& m_node_to_orbit;

	// -------------------------------------------------------------------------
	// Data used for upper bounds

	/// Frequency of degrees among unassigned vertices.
	array<uint64_t> m_degree_count;

	/// Number of assigned neighbor vertices for each vertex.
	array<uint64_t> m_num_assigned_neighbors;
	/// Number of unassigned neighbor vertices for each vertex.
	array<uint64_t> m_num_unassigned_neighbors;

	/* -------------------------- BORDER VERTICES -------------------------- */
	// memory used to sort those vertices with some neighbor assigned.
	// sorted by amount of assigned neighbors

	/// The set of border vertices.
	set_array<node> m_border_nodes;
	/// Auxiliary memory to speed up the calculation of the upper bound.
	sorting::countingsort::memory<node> m_sorting_memory;

	/* -------------------------- BORDER VERTICES -------------------------- */

	// -------------------------------------------------------------------------
	// Algorithm control

	/**
	 * @brief Data related to paths in the tree.
	 *
	 * This struct contains:
	 * - Counting of vertices assigned to the arrangement,
	 * - Maximum and minimum number of vertices of level values \f$\pm2\f$ (these
	 * are generous bounds)
	 * - Actual maximum and minimum number of vertices of level values \f$\pm2\f$.
	 */
	struct path_info {
		/// Number of thistle vertices in the path (assigned to the arrangement).
		uint64_t num_thistles;
		/// Number of vertices in the path assigned to the arrangement.
		uint64_t num_assigned_nodes;
		/// Number of vertices in the path assigned to the arrangement with level +2.
		uint64_t num_assigned_nodes_p2;
		/// Number of vertices in the path assigned to the arrangement with level -2.
		uint64_t num_assigned_nodes_m2;

		/// Lower bound on the number of vertices of this path with level +-2 to
		/// be assigned.
		uint64_t min_pm_two;
		/// Upper bound on the number of vertices of this path with level +-2 to
		/// be assigned.
		uint64_t max_pm_two;

		// number of vertices of level +-2 to assign to the arrangement
		// known after the entire propagation of the path is complete
		// (done only for antennas)
		/**
		 * @brief Number of vertices of this path to be assigned with level +2
		 * in the arrangement.
		 *
		 * Known only after propagating level values. Reset when a propagation is
		 * undone.
		 */
		std::optional<uint64_t> nodes_p2_to_assign;
		/**
		 * @brief Number of vertices of this path to be assigned with level -2
		 * in the arrangement.
		 *
		 * Known only after propagating level values. Reset when a propagation is
		 * undone.
		 */
		std::optional<uint64_t> nodes_m2_to_assign;
	};
	/// Control of vertices to assign in the arrangement with a specific level value.
	array<path_info> m_path_info;

	/// First vertex with which to start the algorithm.
	node m_first_node;

	/// The set of assigned nodes.
	array<char> m_is_node_assigned;

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
	set_array<edge, indexer_edge> m_E_p;
	/// The set of edges with one endpoint in the prefix, the other in the suffix.
	set_array<edge, indexer_edge> m_E_ps;
	/// The set of edges fully contained in the suffix.
	set_array<edge, indexer_edge> m_E_s;

	/**
	 * @brief Directional left degree of each assigned vertex.
	 *
	 * This data is only valid for a vertex @e u when @ref is_vertex_assigned
	 * returns true for @e u.
	 */
	array<uint64_t> m_node_left_degree;
	/**
	 * @brief Directional right degree of each assigned vertex.
	 *
	 * This data is only valid for a vertex @e u when @ref is_vertex_assigned
	 * returns true for @e u.
	 */
	array<uint64_t> m_node_right_degree;
	/**
	 * @brief The level value of each assigned vertex.
	 *
	 * This data is only valid for a vertex @e u when @ref is_vertex_assigned
	 * returns true for @e u.
	 */
	array<int64_t> m_node_level;

	/// Cuts in the arrangement
	array<uint64_t> m_cut_values;

	/// The set of predicted level values for all vertices of the tree.
	array<int64_t> m_predicted_LV;
	/**
	 * @brief Origin of a propagation of level values.
	 *
	 * For each vertex @e u, information of the origin of the propagation of level
	 * values that went through @e u. Notice that @e u may be the origin of the
	 * propagation (see @ref lal::detail::DMax::unconstrained::LV_propagation_origin::self).
	 */
	array<LV_propagation_origin> m_predicted_LV__origin;

	/// Does vertex @e u have a valid prediction of level value?
	[[nodiscard]] bool has_valid_LV_prediction(node u) const noexcept
	{ return m_predicted_LV__origin[u] != LV_propagation_origin::none; }

	/// Did a propagation of level values start at vertex @e u?
	[[nodiscard]] bool is_node_a_trigger_of_LV(node u) const noexcept
	{ return m_predicted_LV__origin[u] == LV_propagation_origin::self; }

#if defined __LAL_DEBUG_DMax_Unc_BnB
	// -------------------------------------------------------------------------
	// Display of algorithm's data and progress

	void output_edge_list() const noexcept;
	void output_arrangement() const noexcept;
	void output_invarr(const position p) const noexcept;
	void output_degree_sequence(const position p) const noexcept;
	void output_left_degree_sequence(const position p) const noexcept;
	void output_right_degree_sequence(const position p) const noexcept;
	void output_level_sequence(const position p) const noexcept;
	void output_cut_signature(const position p) const noexcept;
	void output_num_assigned_neighbors() const noexcept;
	void output_num_unassigned_neighbors() const noexcept;
	void output_border_nodes() const noexcept;
	void output_predicted_level_values() const noexcept;
	void output_path_info() const noexcept;
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
