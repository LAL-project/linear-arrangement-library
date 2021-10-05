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
#include <vector>

// lal includes
#include <lal/graphs/directed_graph.hpp>
#include <lal/graphs/tree.hpp>
#include <lal/graphs/free_tree.hpp>

namespace lal {
namespace graphs {

/**
 * @brief Rooted tree graph class.
 *
 * This class provides its users with an abstraction of rooted trees. Rooted
 * trees are free trees in which one vertex has been designated as the root.
 * Furthermore, in the context of this library, these trees' edges are always
 * oriented towards the leaves (away from the root); this is known as an
 * arborescence. Many methods require objects of this class to be valid rooted
 * trees: the object must be a tree (see @ref is_tree), must have a root (see
 * @ref has_root), and be a valid rooted tree (be an arborescence, that is,
 * see @ref is_orientation_valid).
 *
 * Rooted trees can be constructed in two different ways:
 * - Using an already-constructed free tree via a class constructor, where
 * users have to indicate the root (see @ref rooted_tree(const free_tree&, node)).
 * Alternatively, one can use the method @ref init_rooted, which has the same set
 * of parameters.
 * - Adding edge after edge. In this class, as in @ref free_tree, this addition
 * is constrained so that the underlying undirected graph does not contain cycles.
 * Before (or after) the addition of all the edges, it is recommended
 * the root be set using @ref set_root. If the edges have been added in a
 * systematic fashion -- so it is known whether the tree is an arborescence or
 * not --, it is recommended that the validity of the edges' orientation be set
 * by the user via method @ref set_valid_orientation. If it is not known (or it
 * can't be guaranteed) that the resulting tree is an arborescence, use
 * method @ref find_edge_orientation.
 *
 * Adding edges one by one has a serious drawback. In case the edges do not
 * have a consistent orientation (either always pointing away from the root
 * or always pointing towards it), the resulting graph is not considered to be
 * a valid rooted tree (see @ref is_rooted_tree). Due to efficiency reasons,
 * orientation of edges is not checked before or after their addition. Recall
 * that removal of edges is allowed at every moment.
 *
 * The root allows defining further properties on these graphs. For example,
 * the user can query information regarding subtrees of a particular rooted tree
 * (see methods @ref get_num_nodes_subtree, @ref calculate_size_subtrees,
 * @ref get_edges_subtree, and @ref get_subtree).
 *
 * This class allows flexibility of use of rooted trees regarding the root's
 * choice. Method @ref set_root allows changing the root of rooted trees
 * multiple times and at any time. However, any information dependent
 * on the root becomes invalid upon any change of the root. This information
 * includes, and may not be limited to, the type of rooted tree and the size
 * of the subtrees (see @ref get_num_nodes_subtree). For this reason, is is strongly
 * recommended to build a free tree first and use the constructor
 * @ref rooted_tree(const free_tree&, node), or the method @ref init_rooted,
 * in order to build rooted trees.
 */
class rooted_tree : public directed_graph, virtual public tree {
public:
	/* CONSTRUCTORS */

	/// Empty constructor.
	rooted_tree() noexcept : tree(), directed_graph() { }
	/**
	 * @brief Constructor with number of nodes and root node.
	 * @param n Number of vertices.
	 */
	rooted_tree(uint64_t n) noexcept {
		rooted_tree::_init(n);
	}
	/**
	 * @brief Copy constructor.
	 * @param r Rooted tree.
	 */
	rooted_tree(const rooted_tree& r) noexcept : graph(), tree(), directed_graph() {
		copy_full_rooted_tree(r);
	}
#ifndef SWIG
	/**
	 * @brief Move constructor.
	 * @param r Rooted tree.
	 */
	rooted_tree(rooted_tree&& r) noexcept {
		move_full_rooted_tree(std::move(r));
	}
#endif
	/// Constructor with tree and root node.
	rooted_tree(const free_tree& t, node r) noexcept {
		init_rooted(t, r);
	}
	/// Destructor
	virtual ~rooted_tree() noexcept { }

	/* OPERATORS */

#ifndef SWIG
	/**
	 * @brief Copy assignment operator.
	 * @param r Rooted tree.
	 */
	rooted_tree& operator= (const rooted_tree& r) noexcept {
		copy_full_rooted_tree(r);
		return *this;
	}
	/**
	 * @brief Move assignment operator.
	 * @param r Rooted tree.
	 */
	rooted_tree& operator= (rooted_tree&& r) noexcept {
		move_full_rooted_tree(std::move(r));
		return *this;
	}
#endif

	/* MODIFIERS */

	/**
	 * @brief Adds an edge to the tree.
	 *
	 * This operation checks that the edge added does not produce cycles
	 * only in a @e debug compilation of the library. For a more controlled
	 * addition of the edges, see @ref can_add_edge.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_add is
	 * called after the edge has been added.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Should the graph be normalised?
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre \f$s \neq t\f$
	 * @pre Edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	rooted_tree& add_edge
	(node s, node t, bool norm = true, bool check_norm = true) noexcept;

	/**
	 * @brief Adds an edge to the graph.
	 *
	 * This method only adds an edge, and does no other work: normalisation
	 * is not checked, and no extra work per edge is done.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @pre \f$u \neq v\f$.
	 * @pre The edge \f$\{s,t\}\f$ is not part of the graph.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	rooted_tree& add_edge_bulk(node s, node t) noexcept;

	/**
	 * @brief Finishes adding edges in bulk.
	 * @param norm Normalise the tree.
	 * @param check Check whether the tree is normalised or not.
	 * @pre All edges have been added.
	 */
	void finish_bulk_add(bool norm = true, bool check = true) noexcept;

	/**
	 * @brief Adds a list of edges to the graph.
	 *
	 * This function checks that edges will not produce cycles only in a
	 * @e debug compilation of the library. Moreover, this operation is
	 * faster than calling @ref add_edge since the edges are added in bulk.
	 * For a more controlled addition of the edges, see @ref can_add_edges.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_add is
	 * called after each edge has been added.
	 * @param edges The edges to be added.
	 * @param norm Normalise the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge.
	 * @pre None of the subsets of the list of edges can produce cycles
	 * when added.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edges.
	 */
	rooted_tree& add_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Sets the edges to the graph.
	 *
	 * Sets the edges of this graph assuming that the nodes indexed in the
	 * list are, at most, the number of nodes of this graph.
	 *
	 * This list of edges is assumed to be all the edges that are going
	 * to be added to this graph. This means that the internal data structures
	 * are constructed more efficiently than when adding edges one by one
	 * (see @ref add_edge) or in several chunks (see @ref add_edges).
	 * For a more controlled addition of the edges, see @ref can_add_edges.
	 *
	 * Moreover, the current structure of the graph is cleared before setting
	 * the new edges.
	 * @param edges The edges to be added.
	 * @param norm Normalise the graph after the insertions.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The graph has been initialized with as many nodes as vertices in the
	 * list of edges.
	 * @pre There are no repeated edges in the list.
	 * @pre The list of edges must form a valid rooted tree, i.e., there must
	 * be a unique vertex with no in-going edges, and there must be no cycles.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The tree has a valid root which is, potentially, different from
	 * the previous root it had. Therefore, method @ref has_root returns true.
	 * @post The tree has a valid edge orientation, i.e., method @ref is_orientation_valid
	 * returns true.
	 * @post Method @ref is_rooted_tree returns true.
	 */
	rooted_tree& set_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_remove is
	 * called after the edge has been removed.
	 * @param s Valid node index: \f$0 \le s < n\f$.
	 * @param t Valid node index: \f$0 \le t < n\f$.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre The edge must exist.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The validity of rooted tree and the size of the subtrees are
	 * invalidated, i.e., methods @ref is_orientation_valid and
	 * @ref are_size_subtrees_valid return false.
	 */
	rooted_tree& remove_edge
	(node s, node t, bool norm = false, bool check_norm = true) noexcept;

	/**
	 * @brief Remove an edge from this graph.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed
	 * in bulk.
	 *
	 * <b>For developers:</b> method @ref graph::extra_work_per_edge_remove is
	 * called after each edge has been removed.
	 * @param edges The edges to be deleted.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @pre All the edges in @e edges must meet the precondition of method
	 * @ref add_edge(node,node,bool,bool).
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 * @post The validity of the rooted tree and the size of the subtrees
	 * are invalidated, i.e., methods @ref is_orientation_valid and
	 * @ref are_size_subtrees_valid return false.
	 */
	rooted_tree& remove_edges
	(const std::vector<edge>& edges, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Remove all edges incident to a given vertex.
	 *
	 * This operation is faster than removing edges one by one with
	 * @ref remove_edge(node,node,bool,bool) since the edges are removed
	 * in bulk.
	 *
	 * <b>For developers:</b> method
	 * @ref lal::graphs::graph::extra_work_per_edge_remove is called after each
	 * edge has been removed.
	 * @param u The node whose incident vertices are to be removed.
	 * @param norm Normalise the graph after the deletion.
	 * @param check_norm If @e norm is false then, should we check whether
	 * the result is normalised or not? This might be useful in case the
	 * resulting graph is normalised. If @e norm is true then @e check_norm
	 * is ignored.
	 * @post If @e norm is true the graph is guaranteed to be normalised
	 * after the addition of the edge.
	 */
	virtual rooted_tree& remove_edges_incident_to
	(node u, bool norm = true, bool check_norm = true)
	noexcept;

	/**
	 * @brief Disjoint union of trees.
	 *
	 * Append a rooted tree to this tree. All the nodes in @e t are relabelled
	 * starting at @e n, the number of nodes of the current tree. If the
	 * current graph has no vertices, then the contents of @e t are simply
	 * copied into this graph.
	 *
	 * @param t Input tree.
	 * @param connect_roots The root of the current tree and the root of
	 * @e t are joined by an edge.
	 * @pre If @e connect_roots is true then both trees need to have a root
	 * (see method @ref has_root).
	 * @post The root (if set) of the current tree is kept.
	 * @post Copying the edges of @e t into this tree retains their original
	 * orientation.
	 * @post The size of the subtrees might need recalculating:
	 * - If method @ref are_size_subtrees_valid() returns true for both trees then
	 * the subtree sizes are updated and do not need to be recalculated and
	 * method @ref are_size_subtrees_valid() still returns true.
	 * - If for one of the two graphs the method @ref are_size_subtrees_valid()
	 * returns false then it will still return false after this operation.
	 * @post The graph resulting from the union is normalised only if the
	 * two graphs were normalised prior to the union.
	 */
	void disjoint_union(const rooted_tree& t, bool connect_roots = true)
	noexcept;

	/**
	 * @brief Finds the orientation of the edges.
	 *
	 * It is mandatory that this tree be an arborescence. Therefore, when
	 * the tree has been built by adding edges (see @ref add_edge, @ref add_edges),
	 * the user must tell this class whether what has been built is an
	 * arborescence or not. One can do this by calling method
	 * @ref find_edge_orientation or by setting the type directly using
	 * method @ref set_valid_orientation.
	 *
	 * This method examines the orientation of the tree's edges with respect
	 * to the root and to the leaves, i.e., it determines whether all edges
	 * are oriented towards the leaves (away from the root).
	 * @returns True if the tree is an arborescence. Returns false
	 * otherwise.
	 * @pre This object is a tree (see @ref is_tree).
	 * @pre This tree has a root (see @ref has_root).
	 * @post Method @ref is_orientation_valid evaluates to true if the tree is
	 * an arborescence, or to false if it not an arborescence.
	 */
	bool find_edge_orientation() noexcept;

	/**
	 * @brief Sets wether the type of the rooted tree is valid or not.
	 *
	 * It is mandatory that this tree be an arborescence. Therefore, when
	 * the tree has been built by adding edges (see @ref add_edge, add_edges),
	 * the user must tell this class whether what has been built is an
	 * arborescence or not. One can do this by calling method
	 * @ref find_edge_orientation or by setting the type directly using
	 * method @ref set_valid_orientation.
	 * @param valid Boolean value telling whether the tree is valid or not.
	 * @post Method @ref is_orientation_valid returns the value set by this
	 * function.
	 */
	void set_valid_orientation(bool valid) noexcept {
		m_valid_orientation = valid;
	}

	/**
	 * @brief Initialiser with tree and root node.
	 *
	 * Constructs a rooted tree from a free tree and one of its nodes as
	 * the root of the rooted tree.
	 *
	 * Since the edges are oriented, method @ref is_tree must be true on
	 * parameter @e t (otherwise, some edges might not be reachable from
	 * the root and hence completely undirectable).
	 * @param t Undirected tree.
	 * @param r Root of the directed tree. A node of @e g.
	 * @pre Parameter @e t must be a tree (see @ref is_tree).
	 * @post Method @ref is_rooted_tree returns true.
	 */
	void init_rooted(const free_tree& t, node r) noexcept;

	/**
	 * @brief Calculates the number of nodes at every rooted subtree.
	 * @pre The object must be a tree (see @ref is_tree()).
	 * @pre The tree must have a root (see @ref has_root()).
	 * @post Method @ref are_size_subtrees_valid returns true.
	 */
	void calculate_size_subtrees() noexcept;

	void calculate_tree_type() noexcept;

	/* SETTERS */

	/**
	 * @brief Set the root of this tree.
	 *
	 * Changing the root of a rooted tree invalidates information dependant
	 * on the tree. See the postconditions for details.
	 * @param r Vertex that represents the root.
	 * @post Method @ref has_root returns true.
	 * @post The type of rooted tree and the size of the subtrees are
	 * invalidated.
	 */
	void set_root(node r) noexcept;

	/* GETTERS */

	bool is_rooted() const noexcept { return true; }

	/**
	 * @brief Is this tree a valid rooted tree?
	 *
	 * A tree is a valid rooted tree when:
	 * - the underlying undirected graph is connected and does not contain
	 * cycles (see @ref is_tree()),
	 * - the tree has a root (see @ref has_root, @ref set_root, @ref get_root),
	 * - the orientation of the edges is valid (see @ref is_orientation_valid).
	 *
	 * @returns Whether this tree is a valid rooted tree or not.
	 */
	bool is_rooted_tree() const noexcept
	{ return is_tree() and has_root() and is_orientation_valid(); }

	/**
	 * @brief Is the orientation of the edges valid?
	 *
	 * The edges' orientation is valid if they are all oriented towards the
	 * leaves (away from the root).
	 *
	 * This function returns the value of private attribute @ref m_valid_orientation.
	 * @returns The whether the orientation is valid or not.
	 */
	bool is_orientation_valid() const noexcept { return m_valid_orientation; }

	/// Return the root of this tree.
	node get_root() const noexcept {
#if defined DEBUG
		assert(has_root());
#endif
		return m_root;
	}
	/// Returns whether this rooted tree's root has been set or not
	/// (see @ref set_root).
	bool has_root() const noexcept { return m_has_root; }

	/**
	 * @brief Get the size of a subtree rooted at a given node.
	 * @param u Vertex of the tree.
	 * @returns The number of nodes of the subtree rooted at @e u.
	 * @pre Method @ref are_size_subtrees_valid returns true.
	 */
	uint64_t get_num_nodes_subtree(node u) const noexcept {
#if defined DEBUG
		assert(has_node(u));
#endif
		return m_size_subtrees[u];
	}

	/**
	 * @brief Is a recalculation of the subtree's sizes needed?
	 *
	 * If the method returns @e false then the user should call
	 * @ref calculate_size_subtrees so that the size of every rooted subtree
	 * is recalculated. This information must be calculated prior to calling
	 * many functions of this library.
	 * @returns Whether @ref m_size_subtrees should be recalculated
	 * or not.
	 */
	bool are_size_subtrees_valid() const noexcept
	{ return m_are_size_subtrees_valid; }

	/**
	 * @brief Retrieve the edges of the subtree rooted at @e u.
	 *
	 * The list of edges returned contains labels that depend on the parameter
	 * @e relab. If @e relab is true then the nodes are relabelled to
	 * numbers in \f$[0, n_u)\f$, where \f$n_u\f$ is the number of nodes
	 * of the subtree rooted at @e u, rather than keeping the original
	 * labelling of numbers in \f$[0,n)\f$, where @e n is the number of
	 * nodes of the tree.
	 *
	 * In case of directed trees, the subtree is extracted regardless of the
	 * orientation of the edges. For example, consider the following
	 * complete binary tree of 7 nodes, whose edges are
	 * <pre>
	 * 0 -> 1 -> 3
	 *        -> 4
	 *   -> 2 -> 5
	 *        -> 6
	 * </pre>
	 * The edges of the subtree rooted at 1 are "1 -> 3" and "1 -> 4".
	 * Moreover, the orientation of the edges in the new tree is kept.
	 *
	 * This method can be seen as a way of relabelling nodes when @e u is
	 * the root of the tree and @e relab is true.
	 * @param u Root node of the subtree.
	 * @param relab Should the nodes be relabelled?
	 * @returns A list of edges.
	 * @pre The object must be a valid rooted tree (see @ref is_rooted_tree).
	 * @post Whenever @e relab is true, the label of the first node of
	 * the first edge is guaranteed to be node '0'.
	 */
	std::vector<edge> get_edges_subtree(node u, bool relab = false) const noexcept;

	/**
	 * @brief Retrieve the subtree rooted at node @e u.
	 * @param u Root of the subtree.
	 * @returns A tree containing the nodes of the subtree
	 * rooted at node @e u.
	 * @pre The object must be a valid rooted tree (see @ref is_rooted_tree).
	 * @post The subtree keeps the orientation of the edges in the original
	 * tree.
	 */
	rooted_tree get_subtree(node u) const noexcept;

	/**
	 * @brief Converts this rooted tree into a free tree (see @ref free_tree).
	 * @param norm Normalise the tree.
	 * @param check Chech whether the resulting graph is normalised or not.
	 */
	free_tree to_free_tree
	(bool norm = true, bool check = true) const noexcept;

	/**
	 * @brief Converts a rooted tree into a head vector.
	 *
	 * A head vector of an @e n-vertex tree is a list of non-negative integer
	 * numbers. The number at position @e i denotes the parent node of the vertex
	 * at said position. Value '0' denotes the root. In this case, the vertex
	 * corresponding to the value '0' is not labelled as a root.
	 *
	 * Each tree is formatted as a list of whole, positive numbers (including
	 * zero), each representing a node of the tree. The number 0 denotes the root
	 * of the tree, and a number at a certain position indicates its parent node.
	 * For example, when number 4 is at position 9 it means that node 9 has parent
	 * node 4. Therefore, if number 0 is at position 1 it means that node 1 is
	 * the root of the tree. A complete example of such a tree's representation
	 * is the following
	 *
	 *       0 3 4 1 6 3
	 *
	 *
	 * which should be interpreted as
	 *
	 *		(a) predecessor:       0 3 4 1 6 3
	 *		(b) node of the tree:  1 2 3 4 5 6
	 *
	 *
	 * Note that lines like these are not valid:
	 *
	 *		(1) 0 2 2 2 2 2
	 *		(2) 2 0 0
	 *
	 *
	 * Line (1) is not valid due to a self-reference in the second position,
	 * and (2) is not valid since it contains two '0' (i.e., two roots).
	 *
	 * Methods @ref lal::io::read_head_vector read a head vector from a file in
	 * disk.
	 * @returns The head vector representation of this tree.
	 * @pre This tree is a valid rooted tree (see @ref is_rooted_tree).
	 */
	head_vector get_head_vector() const noexcept;

protected:
	/// Root of the tree.
	node m_root = 0;
	/// Has the root been set?
	bool m_has_root = false;

	/// Is the orientation of the edges valid?
	bool m_valid_orientation = false;

	/**
	 * @brief Number of nodes of the subtrees rooted at a certain node.
	 *
	 * Given a node @e u, @ref m_size_subtrees[u] gives the number
	 * of nodes of the subtree rooted at @e u.
	 */
	std::vector<uint64_t> m_size_subtrees;
	/// Are the contents of @ref m_size_subtrees valid?
	bool m_are_size_subtrees_valid = false;

protected:
	/// Initialises memory of @ref rooted_tree, @ref undirected_graph and
	/// @ref graph classes.
	virtual void _init(uint64_t n) noexcept;
	/// Clears the memory of @ref rooted_tree, @ref undirected_graph and
	/// @ref graph classes.
	virtual void _clear() noexcept;

	void call_union_find_after_add(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) noexcept;
	void call_union_find_after_add(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) const noexcept;
	void call_union_find_after_remove(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) noexcept;
	void call_union_find_after_remove(
		node u, node v,
		uint64_t * const root_of,
		uint64_t * const root_size
	) const noexcept;

	/// Copies all members of this class and the parent class.
	void copy_full_rooted_tree(const rooted_tree& r) noexcept;
	/// Moves all members of this class and the parent class.
	void move_full_rooted_tree(rooted_tree&& r) noexcept;

private:
	using directed_graph::disjoint_union;
};

} // -- namespace graphs
} // -- namespace lal
