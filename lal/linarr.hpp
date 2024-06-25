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

#include <lal/linarr/aggregations/1level.hpp>
#include <lal/linarr/aggregations/2level.hpp>

#include <lal/linarr/C/algorithms_C.hpp>
#include <lal/linarr/C/C.hpp>

#include <lal/linarr/chunking/algorithms.hpp>
#include <lal/linarr/chunking/chunk.hpp>
#include <lal/linarr/chunking/chunking.hpp>
#include <lal/linarr/chunking/output.hpp>

#include <lal/linarr/D/algorithms_Dmin.hpp>
#include <lal/linarr/D/algorithms_Dmin_planar.hpp>
#include <lal/linarr/D/algorithms_Dmin_projective.hpp>
#include <lal/linarr/D/D.hpp>
#include <lal/linarr/D/DMax.hpp>
#include <lal/linarr/D/Dmin.hpp>

#include <lal/linarr/syntactic_dependency_tree/classify.hpp>
#include <lal/linarr/syntactic_dependency_tree/type.hpp>

#include <lal/linarr/dependency_flux.hpp>
#include <lal/linarr/dependency_flux_compute.hpp>

#include <lal/linarr/formal_constraints.hpp>

#include <lal/linarr/head_initial.hpp>
