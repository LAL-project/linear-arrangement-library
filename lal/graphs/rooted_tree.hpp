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
 * trees are free trees in which one vertex has been designates as the root.
 * Furthermore, in the context of this library, these trees' edges are directed.
 * Depending on the orientation of the edges with respect to the root, a rooted
 * tree can be of two types:
 * - @ref rtree_type::arborescence, in which the edges are oriented away from
 * the root, or
 * - @ref rtree_type::anti_arborescence, in which the edges are oriented towards
 * the root.
 *
 * Rooted trees can be constructed in two different ways:
 * - Using an already-constructed free tree via a class constructor, where
 * users have to indicate the root and the type of the rooted tree (see
 * @ref rtree(const ftree&, node, rtree_type)). Alternatively, one can use
 * the method @ref init_rooted, which has the same set of parameters.
 * - Adding edge after edge. In this class, as in @ref ftree, this addition is
 * constrained so that the graph the underlying undirected graph does not contain
 * cycles. Before, or after, the addition of all the edges, it is recommended
 * the root be set using @ref set_root. If the edges have been added in a
 * systematic fashion and the type of rooted tree is known, it is recommended
 * to be set by the user via method @ref set_rtree_type. If it is unkown, use
 * method @ref find_rtree_type.
 *
 * Adding edges one by one has a serious drawback. In case the edges do not
 * have a consistent orientation (either always pointing away from the root
 * or always pointing towards it), the resulting graph is not considered to be
 * a valid rooted tree (see @ref is_rooted_tree). Due to efficiency reasons,
 * orientation of edges is not checked before or after their addition. Recall
 * that removal of edges is allwed at every moment.
 *
 * The root allows defining further properties on these graphs. For example,
 * the user can query information regarding subtrees of a particular rooted tree
 * (see methods @ref n_nodes_subtree and @ref recalc_size_subtrees).
 *
 * This class allows flexibility of use of rooted trees regarding the root's
 * choice. Method @ref set_root allows changing the root of rooted trees
 * multiple times and at any time. However, any information dependent
 * on the root becomes invalid upon any change of the root. This information
 * includes, and may not be lmited to, the type of rooted tree (see
 * @ref m_rtree_type) and the size of the subtrees (see @ref m_size_subtrees).
 * For this reason, is is strongly recommended to build a free tree first and
 * use the constructor @ref rtree(const ftree&, node, rtree_type), or the
 * method @ref init_rooted, in order to build rooted trees.
 */
class rooted_tree : public directed_graph, virtual public tree {
	public:
		/**
		 * @brief Types of rooted directed trees.
		 *
		 * In this library, we distinguish between three disjoint types of
		 * directed trees:
		 * - Arborescences (see @ref rtree_type::arborescence).
		 * - Anti-arborescences (see @ref rtree_type::anti_arborescence).
		 * - None of the above (see @ref rtree_type::none).
		 */
		enum class rtree_type {
			/**
			 * An arboresence is a rooted directed tree in which all the edges
			 * point away from the root, i.e., towards the leaves.
			 */
			arborescence,
			/**
			 * An anti-arboresence is a rooted directed tree in which all the
			 * edges point towards the root, i.e., away from the leaves.
			 */
			anti_arborescence,
			/**
			 * If the direction of the edges of this tree is not uniform
			 * (it is uniform when they always point towards or outwards the
			 * root), the tree has no particular type.
			 */
			none
		};

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
		rooted_tree(const free_tree& t, node r, rtree_type arb = rtree_type::arborescence);
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
		 * resulting graph is normalised.
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
		 * resulting graph is normalised.
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
		 * resulting graph is normalised.
		 * @pre The edge must exist.
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 * @post The type of rooted tree and the size of the subtrees are
		 * invalidated, i.e., method @ref rtree_type_valid returns false
		 * and @ref need_recalc_size_subtrees returns true.
		 */
		rooted_tree& remove_edge
		(node s, node t, bool norm = true, bool check_norm = true);

		/**
		 * @brief Remove an edge from this graph.
		 *
		 * This operation is faster than removing edges one by one with
		 * @ref remove_edge(node,node,bool) since the edges are removed in bulk.
		 * @param edges The edges to be deleted.
		 * @param norm Normalise the graph after the deletion.
		 * @param check_norm If @e norm is false then, should we check whether
		 * the result is normalised or not? This might be useful in case the
		 * resulting graph is normalised.
		 * @pre All the edges in @e edges must meet the precondition of method
		 * @ref add_edge(node,node,bool).
		 * @post If @e norm is true the graph is guaranteed to be normalised
		 * after the addition of the edge.
		 * @post The type of rooted tree and the size of the subtrees are
		 * invalidated, i.e., method @ref rtree_type_valid returns false
		 * and @ref need_recalc_size_subtrees returns true.
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
		 * @ref need_recalc_size_subtrees() returns true).
		 */
		void disjoint_union(const rooted_tree& t, bool connect_roots = true);

		/**
		 * @brief Calculates the type of directed rooted tree.
		 *
		 * Examines the orientation of the tree with respect to the root and
		 * to the leaves. Then, determines the tree's type (see @ref rtree_type)
		 * according to this orientation.
		 *
		 * If the tree has only one vertex the type is @ref rtree_type::arborescence.
		 * @return Returns true if the type is either @ref rtree_type::arborescence
		 * or rtree_type::anti_arborescence. Returns false if the type is
		 * @ref rtree_type::none.
		 * @pre This object is a tree (see @ref is_tree).
		 * @pre This tree has a root (see @ref has_root).
		 * @post Method @ref rtree_type_valid evaluates to true if the tree is
		 * an rtree_type::arborescence or an rtree_type::anti_arborescence.
		 */
		bool find_rtree_type();

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
		 * @param arb The type of directed rooted tree.
		 * @pre Parameter @e t must be a tree (see @ref is_tree).
		 * @pre Parameter @e arb must be either @ref rtree_type::arborescence
		 * or @ref rtree_type::anti_arborescence.
		 * @post Method @ref is_rooted_tree returns true.
		 */
		void init_rooted
		(const free_tree& t, node r, rtree_type arb = rtree_type::arborescence);

		/**
		 * @brief Calculates the number of nodes at every rooted subtree.
		 *
		 * The method can traverse the directed tree using reversed edges, i.e.,
		 * from a root node 's' the method can follow out-edges (of the form
		 * s->t) and in-edges (of the form t->s). If the tree is an
		 * @ref rtree_type::anti_arborescence the method follows reversed edges.
		 * If the tree is an @ref rtree_type::arborescence the method follows
		 * "regular" edges.
		 * @pre The object must be a tree (see @ref is_tree()).
		 * @pre The tree must have a root (see @ref has_root()).
		 * @pre In case @e rev is true, method @ref rtree_type_valid must
		 * return true and the tree must be an @ref rtree_type::arborescence
		 * or an an @ref rtree_type::anti_arborescence.
		 * @post Method @ref need_recalc_size_subtrees returns false.
		 */
		void recalc_size_subtrees();

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

		/**
		 * @brief Sets the type of rooted tree.
		 *
		 * It is mandatory to call this method after calling @ref set_root, in
		 * case that method is ever called.
		 * @param type Type of the tree.
		 * @pre The type of tree in @e type must match the actual type of the
		 * underlying rooted tree.
		 * @post Method @ref rtree_type_valid returns true.
		 */
		void set_rtree_type(const rtree_type& type);

		/* GETTERS */

		/**
		 * @brief Can this edge be added?
		 *
		 * In a tree, this edge can only be added if it does not produce cycles.
		 * @param s First node of the edge.
		 * @param t Second node of the edge.
		 * @return Returns whether the addition of this new edge can be added
		 * to the tree without producing cycles.
		 */
		bool can_add_edge(node s, node t) const;
		/**
		 * @brief Can these edges be added?
		 *
		 * In a tree, these edges can only be added if their addition to the
		 * tree do not produce cycles.
		 * @param edges List of edges.
		 * @return Returns whether the addition of these new edges can be added
		 * to the tree without producing cycles.
		 */
		bool can_add_edges(const std::vector<edge>& edges) const;

		bool is_rooted() const;

		/**
		 * @brief Is this tree a valid rooted tree?
		 *
		 * A tree is a valid rooted tree when:
		 * - the underlying undirected graph is connected and does not contain
		 * cycles (see @ref is_tree()),
		 * - the tree has a root (see @ref has_root, @ref set_root, @ref get_root),
		 * - the type of rooted tree is valid (see @ref rtree_type_valid) and
		 * the rooted tree type is either @ref rtree_type::arborescence, or
		 * @ref rtree_type::anti_arborescence.
		 * @return Returns whether this tree is a valid rooted tree or not.
		 */
		bool is_rooted_tree() const;

		/**
		 * @brief Returns the type of this rooted tree.
		 * @pre Method @ref rtree_type_valid returns true.
		 */
		rtree_type get_rtree_type() const;
		/// Is the rooted type valid?
		bool rtree_type_valid() const;

		/// Return the root of this tree.
		node get_root() const;
		/// Returns whether this rooted tree's root has been set or not
		/// (see @ref set_root).
		bool has_root() const;

		/**
		 * @brief Returns the number of nodes of the subtree rooted at @e u.
		 * @param u Vertex of the tree.
		 * @return Returns @ref m_size_subtrees[u].
		 * @pre Method @ref need_recalc_size_subtrees returns false.
		 */
		uint32_t n_nodes_subtree(node u) const;

		/**
		 * @brief Is a recalculation of the subtree's sizes needed?
		 *
		 * If the method returns true, then call @ref recalc_size_subtrees.
		 * @return Returns whether @ref m_size_subtrees should be recalculated
		 * or not.
		 */
		bool need_recalc_size_subtrees() const;

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

		/**
		 * @brief Type of rooted directed tree.
		 *
		 * This parameter is decided during the construction of the tree via
		 * constructor @ref rtree(const ftree&,node,rtree_type), via calling
		 * method @ref find_rtree_type(), or given by the user in
		 * @ref set_rtree_type.
		 */
		rtree_type m_rtree_type = rtree_type::none;
		/// Are the contents of @ref m_rtree_type valid?
		bool m_rtree_type_valid = false;

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
		/// Initialises memory of @ref rtree, @ref ugraph and @ref graph classes.
		virtual void _init(uint32_t n);
		/// Clears the memory of @ref rtree, @ref ugraph and @ref graph classes.
		virtual void _clear();

	private:
		using directed_graph::disjoint_union;
};

/// Shorthand for @ref rooted_graph.
typedef rooted_tree rtree;

} // -- namespace graphs
} // -- namespace lal
