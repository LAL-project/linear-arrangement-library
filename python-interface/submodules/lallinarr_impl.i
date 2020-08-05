
%{

#include <lal/graphs.hpp>
#include <lal/linarr.hpp>

%}

%include "../lal/linarr/1level.hpp"
%include "../lal/linarr/2level.hpp"
%include "../lal/linarr/C.hpp"
%include "../lal/linarr/D.hpp"
%include "../lal/linarr/Dmin.hpp"
%include "../lal/linarr/headedness.hpp"
%include "../lal/linarr/tree_structure_class.hpp"

// --------------------------------
// Extendind the function templates

%template(__MDD_1level_rational_undirected_graph)	lal::linarr::MDD_1level_rational<lal::graphs::undirected_graph>;
%template(__MDD_1level_rational_directed_graph)		lal::linarr::MDD_1level_rational<lal::graphs::directed_graph>;
%template(__MDD_1level_rational_free_tree)			lal::linarr::MDD_1level_rational<lal::graphs::free_tree>;
%template(__MDD_1level_rational_rooted_tree)		lal::linarr::MDD_1level_rational<lal::graphs::rooted_tree>;

%template(__MDD_1level_undirected_graph)	lal::linarr::MDD_1level<lal::graphs::undirected_graph>;
%template(__MDD_1level_directed_graph)		lal::linarr::MDD_1level<lal::graphs::directed_graph>;
%template(__MDD_1level_free_tree)			lal::linarr::MDD_1level<lal::graphs::free_tree>;
%template(__MDD_1level_rooted_tree)			lal::linarr::MDD_1level<lal::graphs::rooted_tree>;

%template(__MDD_2level_rational_undirected_graph)	lal::linarr::MDD_2level_rational<lal::graphs::undirected_graph>;
%template(__MDD_2level_rational_directed_graph)		lal::linarr::MDD_2level_rational<lal::graphs::directed_graph>;
%template(__MDD_2level_rational_free_tree)			lal::linarr::MDD_2level_rational<lal::graphs::free_tree>;
%template(__MDD_2level_rational_rooted_tree)		lal::linarr::MDD_2level_rational<lal::graphs::rooted_tree>;

%template(__MDD_2level_undirected_graph)	lal::linarr::MDD_2level<lal::graphs::undirected_graph>;
%template(__MDD_2level_directed_graph)		lal::linarr::MDD_2level<lal::graphs::directed_graph>;
%template(__MDD_2level_free_tree)			lal::linarr::MDD_2level<lal::graphs::free_tree>;
%template(__MDD_2level_rooted_tree)			lal::linarr::MDD_2level<lal::graphs::rooted_tree>;

%pythoncode %{
def MDD_1level_rational(g, l):
	return __dict__[ "__MDD_1level_rational_" + str(type(g))[7:] ](g, l)

def MDD_1level(g, l):
	return __dict__[ "__MDD_1level_" + str(type(g))[7:] ](g, l)

def MDD_2level_rational(g, l):
	return __dict__[ "__MDD_2level_rational_" + str(type(g))[7:] ](g, l)

def MDD_2level(g, l):
	return __dict__[ "__MDD_2level_" + str(type(g))[7:] ](g, l)
%}
