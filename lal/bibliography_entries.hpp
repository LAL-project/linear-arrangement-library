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

#pragma once

// C++ includes
#include <cstddef>

namespace lal {

#if defined LAL_REGISTER_BIBLIOGRAPHY

/// Total number of entries in the bibliography.
static constexpr std::size_t num_bib_entries = 50;

/// A type-safe list of bibliographic entries.
enum class bib_entries {
	/// Entry \cite Alemany2024b.
	Alemany2024b,
	/// Entry \cite Alemany2024a.
	Alemany2024a,
	/// Entry \cite Alemany2023a.
	Alemany2023a,
	/// Entry \cite Alemany2022a.
	Alemany2022a,
	/// Entry \cite Alemany2022b.
	Alemany2022b,
	/// Entry \cite Alemany2022c.
	Alemany2022c,
	/// Entry \cite Anderson2021a.
	Anderson2021a,
	/// Entry \cite Macutek2021a.
	Macutek2021a,
	/// Entry \cite Alemany2020a.
	Alemany2020a,
	/// Entry \cite Alemany2020b.
	Alemany2020b,
	/// Entry \cite Johnson2020a.
	Johnson2020a,
	/// Entry \cite Alemany2019a.
	Alemany2019a,
	/// Entry \cite Anderson2019a.
	Anderson2019a,
	/// Entry \cite English2019a.
	English2019a,
	/// Entry \cite Ferrer2019a.
	Ferrer2019a,
	/// Entry \cite Nurse2019a.
	Nurse2019a,
	/// Entry \cite Ferrer2018a.
	Ferrer2018a,
	/// Entry \cite Nurse2018a.
	Nurse2018a,
	/// Entry \cite Esteban2017a.
	Esteban2017a,
	/// Entry \cite Kahane2017a.
	Kahane2017a,
	/// Entry \cite Futrell2015a.
	Futrell2015a,
	/// Entry \cite Jing2015a.
	Jing2015a,
	/// Entry \cite Ferrer2014a.
	Ferrer2014a,
	/// Entry \cite Pitler2013a.
	Pitler2013a,
	/// Entry \cite Gomez2011a.
	Gomez2011a,
	/// Entry \cite Liu2010a.
	Liu2010a,
	/// Entry \cite Gildea2007a.
	Gildea2007a,
	/// Entry \cite Ferrer2004a.
	Ferrer2004a,
	/// Entry \cite Hochberg2003a.
	Hochberg2003a,
	/// Entry \cite Cormen2001a.
	Cormen2001a,
	/// Entry \cite Iordanskii1987a.
	Iordanskii1987a,
	/// Entry \cite Wright1986a.
	Wright1986a,
	/// Entry \cite Chung1984a.
	Chung1984a,
	/// Entry \cite Colbourn1981a.
	Colbourn1981a,
	/// Entry \cite Wilf1981a.
	Wilf1981a,
	/// Entry \cite Beyer1980a.
	Beyer1980a,
	/// Entry \cite Shiloach1979a.
	Shiloach1979a,
	/// Entry \cite Nijenhuis1978a.
	Nijenhuis1978a,
	/// Entry \cite Aho1974a.
	Aho1974a,
	/// Entry \cite Harary1973a.
	Harary1973a,
	/// Entry \cite Karp1972a.
	Karp1972a,
	/// Entry \cite Harary1969a.
	Harary1969a,
	/// Entry \cite Otter1948a.
	Otter1948a,
	/// Entry \cite Pruefer1918a.
	Pruefer1918a,
	/// Entry \cite OEIS_A000055.
	OEIS_A000055,
	/// Entry \cite OEIS_A000081.
	OEIS_A000081,
	/// Entry \cite OEIS_kLinearTrees.
	OEIS_kLinearTrees,
	/// Entry \cite GMP_library.
	GMP_library,
	/// Entry \cite MathWorld_SpiderGraph.
	MathWorld_SpiderGraph,
	/// Entry \cite GiacXcas_Manual.
	GiacXcas_Manual,
};

#endif

} // namespace lal
