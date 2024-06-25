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
#include <lal/linear_arrangement.hpp>
#include <lal/graphs/graph.hpp>
#include <lal/properties/bipartite_graph_coloring.hpp>
#include <lal/properties/bipartite_graph_colorability.hpp>

namespace lal {
namespace generate {

/**
 * @brief Exhaustive enumeration of all bipartite arrangements of any bipartite graph.
 *
 * This class generates all \f$|V_1|!\cdot|V_2|!\f$ bipartite arrangements of a
 * bipartite graph \f$B=(V_1\cup V_2, E)\f$ (see page @ref LAL_concepts__linear_arrangement__types
 * for a definition of bipartite arrangements). Unlike other generators
 * (e.g. @ref lal::generate::all_projective_arrangements), this class can be
 * instantiated with the coloring of the bipartite graph, from which the "blue"
 * and "red" vertices are extracted. However, constructing this class with a graph
 * is allowed for the sake of consistency; neverthless, only its number of vertices
 * is actually needed. This is due to the simple fact that the graph structure
 * does not matter for the enumeration of these arrangements.
 *
 * In order to use this class, you must first provide the coloring of the graph.
 * Arrangements are generated internally, i.e., arragements are encoded in the
 * internal state of the generator. Said state is updated using method @ref next(),
 * which updates it to encode the next arrangement in the generation. In order
 * to retrieve an arrangement, use method @ref get_arrangement(). Upon initialisation,
 * the generator encodes the first arrangement, which has to be retrieved using
 * method @ref get_arrangement().
 *
 * This class is a wrapper over the C++'s std::next_permutation algorithm.
 *
 * A possible usage of this class is the following:
 * @code
 *		lal::generate::all_bipartite_arrangements Gen(g); // t is any graph (tree included)
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.get_arrangement();
 *			// ...
 *			Gen.next();
 *		}
 * @endcode
 * When using @ref get_arrangement() in combination with @ref next(), it is important
 * to bear in mind that, if the arrangement is declared as a constant reference,
 * users should <b>not</b> call @ref next() before using the retrieved arrangement.
 * To alleviate this source of bugs, this class can also be used in a for loop:
 * @code
 *		for (lal::generate::all_bipartite_arrangements Gen(g); not Gen.end(); Gen.next()) {
 *			const lal::linear_arrangement arr = Gen.get_arrangement();
 *			// ...
 *		}
 * @endcode
 * This class also has method @ref yield_arrangement() which is aimed at avoiding
 * potential source of bugs:
 * @code
 *		lal::generate::all_bipartite_arrangements Gen(g);
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 * Note that the arrangement is not declared as a constant reference since
 * method @ref yield_arrangement() returns the tree as a copy.
 *
 * Furthermore, if the biparite coloring object (@ref lal::properties::bipartite_graph_coloring)
 * of the graph @e g already exists, then one can construct this class using said
 * object.
 * @code
 *		lal::properties::bipartite_graph_coloring c = lal::properties::coloring(g);
 *		lal::generate::all_bipartite_arrangements Gen(c);
 *		while (not Gen.end()) {
 *			const lal::linear_arrangement& arr = Gen.yield_arrangement();
 *			// ...
 *		}
 * @endcode
 */
class all_bipartite_arrangements {
public:

	/**
	 * @brief Constructor with graph.
	 *
	 * This constructor needs to calculate the bipartite coloring of the graph.
	 * @tparam graph_t Graph type. A subclass of lal::graphs::graph.
	 * @param g Input graph.
	 * @pre The input graph @e g is bipartite.
	 */
	template <class graph_t>
	all_bipartite_arrangements(const graph_t& g) {
		static_assert(std::is_base_of_v<lal::graphs::graph, graph_t>);
		m_coloring = properties::bipartite_coloring(g);
		init();
	}

	/**
	 * @brief Default copy constructor.
	 * @param Gen Exhaustive bipartite arrangement generator.
	 */
	all_bipartite_arrangements(const all_bipartite_arrangements& Gen) = default;
	/**
	 * @brief Default move constructor.
	 * @param Gen Exhaustive bipartite arrangement generator.
	 */
	all_bipartite_arrangements(all_bipartite_arrangements&& Gen) = default;

	/**
	 * @brief Constructor with coloring.
	 * @param c Input coloring of a bipartite graph.
	 */
	all_bipartite_arrangements(const properties::bipartite_graph_coloring& c) noexcept {
		m_coloring = c;
		init();
	}
	/**
	 * @brief Constructor with coloring.
	 * @param c Input coloring of a bipartite graph.
	 */
	all_bipartite_arrangements(properties::bipartite_graph_coloring&& c) noexcept {
		m_coloring = std::move(c);
		init();
	}

	/// Default destructor.
	~all_bipartite_arrangements() noexcept = default;

	/**
	 * @brief Returns the current linear arrangemnt.
	 *
	 * Recall that method @ref next() should <b>not</b> be called until the
	 * arrangement has been processed if such an arrangement was declared as a
	 * constant reference.
	 * @returns A permutation of the vertices (a.k.a. a linear arrangement).
	 */
	const linear_arrangement& get_arrangement() const noexcept
	{ return m_arr; }

	/// Returns true if the end of the iteration was reached.
	bool end() const noexcept { return m_do_mirror and m_reached_end_red; }

	/**
	 * @brief Generates the next arrangement.
	 *
	 * Modifies the internal state so that the next arrangement
	 * can be retrieved using method @ref get_arrangement.
	 */
	void next() noexcept;

	/// Sets the generator to its initial state.
	void reset() noexcept {
		m_do_mirror = false;
		m_reached_end_blue = false;
		m_reached_end_red = false;
		init_arrangement(true);
	}

	/**
	 * @brief Constructs the current arrangement.
	 * @return The current arrangement.
	 * @post This generator is moved to the next arrangement.
	 */
	linear_arrangement yield_arrangement() noexcept {
		// yes, a copy...
		const linear_arrangement arr = get_arrangement();
		next();
		return arr;
	}

private:
	/**
	 * @brief Initializes this class.
	 *
	 * Initializes the arrangement (@ref m_arr) and the number of blue and red
	 * vertices (@ref m_n_blue and @ref m_n_red).
	 */
	void init() noexcept;

	/**
	 * @brief Initializes this class.
	 *
	 * Initializes the arrangement by placing the red or blue vertices in the
	 * left side of the arrangement depending on the value of the parameter.
	 * @param red_first Place red vertices first if true.
	 */
	void init_arrangement(bool red_first) noexcept;

private:
	/// Has the end of the iteration been reached for mirrored arrangements?
	bool m_do_mirror;
	/// Has the end of the iteration been reached for blue vertices?
	bool m_reached_end_blue;
	/// Has the end of the iteration been reached for red vertices?
	bool m_reached_end_red;

	/// Number of blue vertices.
	std::size_t m_n_blue;
	/// Number of red vertices.
	std::size_t m_n_red;

	/**
	 * @brief The arrangement generated by this class.
	 *
	 * Actually, generated by the std::next_permutation algorithm.
	 */
	linear_arrangement m_arr;

	/// Coloring of the bipartite graph.
	properties::bipartite_graph_coloring m_coloring;

private:
	/// Shortcut to blue color.
	static constexpr properties::bipartite_graph_coloring::color_t blue =
		properties::bipartite_graph_coloring::blue;
	/// Shortcut to red color.
	static constexpr properties::bipartite_graph_coloring::color_t red =
		properties::bipartite_graph_coloring::red;
};

} // -- namespace generate
} // -- namespace lal
