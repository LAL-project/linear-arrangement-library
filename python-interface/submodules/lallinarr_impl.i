
%{

#include <lal/graphs.hpp>
#include <lal/linarr.hpp>

%}

%include "../lal/linarr/1level.hpp"
%include "../lal/linarr/2level.hpp"
%include "../lal/linarr/C.hpp"
%include "../lal/linarr/D.hpp"
%include "../lal/linarr/dependency_flux.hpp"
%include "../lal/linarr/Dmin.hpp"
%include "../lal/linarr/headedness.hpp"
%include "../lal/linarr/syntactic_dependency_structure_type.hpp"

// --------------------------------
// Extendind the function templates

%template(__MDD_1level_rational_undirected_graph)	lal::linarr::mean_dependency_distance_1level_rational<lal::graphs::undirected_graph>;
%template(__MDD_1level_rational_directed_graph)		lal::linarr::mean_dependency_distance_1level_rational<lal::graphs::directed_graph>;
%template(__MDD_1level_rational_free_tree)			lal::linarr::mean_dependency_distance_1level_rational<lal::graphs::free_tree>;
%template(__MDD_1level_rational_rooted_tree)		lal::linarr::mean_dependency_distance_1level_rational<lal::graphs::rooted_tree>;

%template(__MDD_1level_undirected_graph)			lal::linarr::mean_dependency_distance_1level<lal::graphs::undirected_graph>;
%template(__MDD_1level_directed_graph)				lal::linarr::mean_dependency_distance_1level<lal::graphs::directed_graph>;
%template(__MDD_1level_free_tree)					lal::linarr::mean_dependency_distance_1level<lal::graphs::free_tree>;
%template(__MDD_1level_rooted_tree)					lal::linarr::mean_dependency_distance_1level<lal::graphs::rooted_tree>;

%template(__MDD_2level_rational_undirected_graph)	lal::linarr::mean_dependency_distance_2level_rational<lal::graphs::undirected_graph>;
%template(__MDD_2level_rational_directed_graph)		lal::linarr::mean_dependency_distance_2level_rational<lal::graphs::directed_graph>;
%template(__MDD_2level_rational_free_tree)			lal::linarr::mean_dependency_distance_2level_rational<lal::graphs::free_tree>;
%template(__MDD_2level_rational_rooted_tree)		lal::linarr::mean_dependency_distance_2level_rational<lal::graphs::rooted_tree>;

%template(__MDD_2level_undirected_graph)			lal::linarr::mean_dependency_distance_2level<lal::graphs::undirected_graph>;
%template(__MDD_2level_directed_graph)				lal::linarr::mean_dependency_distance_2level<lal::graphs::directed_graph>;
%template(__MDD_2level_free_tree)					lal::linarr::mean_dependency_distance_2level<lal::graphs::free_tree>;
%template(__MDD_2level_rooted_tree)					lal::linarr::mean_dependency_distance_2level<lal::graphs::rooted_tree>;


%pythoncode %{
def mean_dependency_distance_1level_rational(g, l):
	return __dict__[ "__mean_dependency_distance_1level_rational_" + str(type(g))[7:] ](g, l)

def mean_dependency_distance_1level(g, l):
	return __dict__[ "__mean_dependency_distance_1level_" + str(type(g))[7:] ](g, l)

def mean_dependency_distance_2level_rational(g, l):
	return __dict__[ "__mean_dependency_distance_2level_rational_" + str(type(g))[7:] ](g, l)

def mean_dependency_distance_2level(g, l):
	return __dict__[ "__mean_dependency_distance_2level_" + str(type(g))[7:] ](g, l)
%}
