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

// C++ includes
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
 * (see methods @ref n_nodes_subtree, @ref calculate_size_subtrees,
 * @ref get_edges_subtree, and @ref get_subtree).
 *
 * This class allows flexibility of use of rooted trees regarding the root's
 * choice. Method @ref set_root allows changing the root of rooted trees
 * multiple times and at any time. However, any information dependent
 * on the root becomes invalid upon any change of the root. This information
 * includes, and may not be limited to, the type of rooted tree and the size
 * of the subtrees (see @ref n_nodes_subtree). For this reason, is is strongly
 * recommended to build a free tree first and use the constructor
 * @ref rooted_tree(const free_tree&, node), or the method @ref init_rooted,
 * in order to build rooted trees.
 */
class rooted_tree : public directed_graph, virtual public tree {
	public:
		/// Default constructor.
		rooted_tree() = default;
		/// Default move constructor.
		rooted_tree(rooted_tree&&) = default;
		/// Default copy constructor.
		rooted_tree(const rooted_tree&) = default;
		/// Constructor with number of nodes and root node.
		rooted_tree(uint32_t n);
		/// Constructor with tree and root node.
		rooted_tree(const free_tree& t, node r);
		/// Default destructor
		virtual ~rooted_tree() = default;

		/// Default move assignment operator.
		rooted_tree& operator= (rooted_tree&&) = default;
		/// Default copy assignment operator.
		rooted_tree& operator= (const rooted_tree&) = default;

		/* MODIFIERS */

		/**
		 * @brief Adds an edge to the tree.
		 *
		 * This operation checks that the edge added does not produce cycles,
		 * but only in a @e debug compilation of the library.
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
		(node s, node t, bool norm = false, bool check_norm = true);

		/**
		 * @brief Adds a list of edges to the graph.
		 *
		 * This operation is faster than calling @ref add_edge since the
		 * edges are added in bulk.
		 *
		 * However, unlike function @ref add_edge, this only checks that
		 * the addition of the edges have not produced cycles only after they
		 * have been added.
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
		(const std::vector<edge>& edges, bool norm = true, bool check_norm = true);

		/**
		 * @brief Remove an edge from this graph.
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
		 * @ref size_subtrees_valid return false.
		 */
		rooted_tree& remove_edge
		(node s, node t, bool norm = false, bool check_norm = true);

		/**
		 * @brief Remove an edge from this graph.
		 *
		 * This operation is faster than removing edges one by one with
		 * @ref remove_edge(node,node,bool,bool) since the edges are removed in bulk.
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
		 * @ref size_subtrees_valid return false.
		 */
		rooted_tree& remove_edges
		(const std::vector<edge>& edges, bool norm = true, bool check_norm = true);

		/**
		 * @brief Disjoint union of trees.
		 *
		 * Given a rooted tree, append it to the current tree.
		 *
		 * All the nodes in @e t are relabelled starting at @e n, the number
		 * of nodes of the current tree.
		 *
		 * If the current graph has no vertices, then the contents of @e t
		 * are simply copied into this graph.
		 * @param t Input tree.
		 * @param connect_roots The root of the current tree and the root of
		 * @e t are joined by an edge.
		 * @pre If @e connect_roots is true then the current needs to have a
		 * root (see method @ref has_root).
		 * @post The root (if set) of the current tree is kept.
		 * @post The type of rooted tree (if set) of the current tree is kept.
		 * @post If @e connect_roots is true then method @ref is_rooted_tree()
		 * returns true. Said method returns false if otherwise.
		 * @post The size of the subtrees needs recalculating (method
		 * @ref size_subtrees_valid() returns true).
		 */
		void disjoint_union(const rooted_tree& t, bool connect_roots = true);

		/**
		 * @brief Finds the orientation of the edges.
		 *
		 * It is mandatory that this tree be an arborescence. Therefore, when
		 * the tree has been built by adding edges (see @ref add_edge, add_edges),
		 * the user must tell this class whether what has been built is an
		 * arborescence or not. One can do this by calling method
		 * @ref find_edge_orientation or by setting the type directly using
		 * method @ref set_valid_orientation.
		 *
		 * This method examines the orientation of the tree's edges with respect
		 * to the root and to the leaves, i.e., it determines whether all edges
		 * are oriented towards the leaves (away from the root).
		 * @return Returns true if the tree is an arborescence. Returns false
		 * otherwise.
		 * @pre This object is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 * @post Method @ref is_orientation_valid evaluates to true if the tree is
		 * an arborescence, or to false if it not an arborescence.
		 */
		bool find_edge_orientation();

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
		void set_valid_orientation(bool valid);

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
		void init_rooted(const free_tree& t, node r);

		/**
		 * @brief Calculates the number of nodes at every rooted subtree.
		 * @pre The object must be a tree (see @ref is_tree()).
		 * @pre The tree must have a root (see @ref has_root()).
		 * @post Method @ref size_subtrees_valid returns false.
		 */
		void calculate_size_subtrees();

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
		void set_root(node r);

		/* GETTERS */

		bool is_rooted() const;

		/**
		 * @brief Is this tree a valid rooted tree?
		 *
		 * A tree is a valid rooted tree when:
		 * - the underlying undirected graph is connected and does not contain
		 * cycles (see @ref is_tree()),
		 * - the tree has a root (see @ref has_root, @ref set_root, @ref get_root),
		 * - the orientation of the edges is valid (see @ref is_orientation_valid).
		 * @return Returns whether this tree is a valid rooted tree or not.
		 */
		bool is_rooted_tree() const;

		/**
		 * @brief Is the orientation of the edges valid?
		 *
		 * The edges' orientation is valid if they are all oriented towards the
		 * leaves (away from the root).
		 * @return Returns @ref m_valid_orientation.
		 */
		bool is_orientation_valid() const;

		/// Return the root of this tree.
		node get_root() const;
		/// Returns whether this rooted tree's root has been set or not
		/// (see @ref set_root).
		bool has_root() const;

		/**
		 * @brief Returns the number of nodes of the subtree rooted at @e u.
		 * @param u Vertex of the tree.
		 * @return Returns @ref m_size_subtrees[u].
		 * @pre Method @ref size_subtrees_valid returns false.
		 */
		uint32_t n_nodes_subtree(node u) const;

		/**
		 * @brief Is a recalculation of the subtree's sizes needed?
		 *
		 * If the method returns true, then call @ref calculate_size_subtrees.
		 * @return Returns whether @ref m_size_subtrees should be recalculated
		 * or not.
		 */
		bool size_subtrees_valid() const;

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
		 * orientation of the edges. For example, consider an anti-arborescence
		 * of a complete binary tree of 7 nodes, whose edges are
		 * <pre>
		 * 0 <- 1
		 *		1 <- 3
		 *		1 <- 4
		 *   <- 2
		 *		2 <- 5
		 *		2 <- 6
		 * </pre>
		 * The edges of the subtree rooted at 1 are "3 -> 1" and "4 -> 1".
		 * Moreover, the orientation of the edges is guaranteed to be
		 * first-node-to-second-node.
		 *
		 * Regardless of the directedness of the graph, this method can be seen
		 * as a way of relabelling nodes when @e u is the root of the tree
		 * and @e relab is true.
		 * @param u Root node of the subtree.
		 * @param relab Should the nodes be relabelled?
		 * @return Returns a list of edges.
		 * @pre The object must be a valid rooted tree (see @ref is_rooted_tree).
		 * @post Whenever @e relab is true, the label of the first node of
		 * the first edge is guaranteed to be node '0'.
		 */
		std::vector<edge> get_edges_subtree(node u, bool relab = false) const;

		/**
		 * @brief Retrieve the subtree rooted at node @e u.
		 * @param u Root of the subtree.
		 * @return Returns a tree containing the nodes of the subtree
		 * rooted at node @e u.
		 * @pre The object must be a valid rooted tree (see @ref is_rooted_tree).
		 * @post The subtree keeps the orientation of the edges in the original
		 * tree.
		 */
		rooted_tree get_subtree(node u) const;

		/// Converts this rooted tree into a free tree (see class @ref tree).
		free_tree to_undirected() const;

	protected:
		/// Root of the tree.
		node m_root;
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
		std::vector<uint32_t> m_size_subtrees;
		/// Are the contents of @ref m_size_subtrees valid?
		bool m_need_recalc_size_subtrees = true;

	protected:
		/// Initialises memory of @ref rooted_tree, @ref undirected_graph and
		/// @ref graph classes.
		virtual void _init(uint32_t n);
		/// Clears the memory of @ref rooted_tree, @ref undirected_graph and
		/// @ref graph classes.
		virtual void _clear();

	private:
		using directed_graph::disjoint_union;
};

} // -- namespace graphs
} // -- namespace lal
