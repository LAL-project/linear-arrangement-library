
%{

#include <lal/graphs.hpp>
#include <lal/linarr.hpp>

%}

%include "../lal/linarr/1level.hpp"
%include "../lal/linarr/2level.hpp"
%include "../lal/linarr/C.hpp"
%include "../lal/linarr/D.hpp"
%include "../lal/linarr/headedness.hpp"
%include "../lal/linarr/tree_structure_class.hpp"

// --------------------------------
// Extendind the function templates

%template(__MDD_1level_rational_ugraph)	lal::linarr::MDD_1level_rational<lal::graphs::ugraph>;
%template(__MDD_1level_rational_dgraph)	lal::linarr::MDD_1level_rational<lal::graphs::dgraph>;
%template(__MDD_1level_rational_ftree)	lal::linarr::MDD_1level_rational<lal::graphs::ftree>;
%template(__MDD_1level_rational_rtree)	lal::linarr::MDD_1level_rational<lal::graphs::rtree>;

%template(__MDD_1level_ugraph)	lal::linarr::MDD_1level<lal::graphs::ugraph>;
%template(__MDD_1level_dgraph)	lal::linarr::MDD_1level<lal::graphs::dgraph>;
%template(__MDD_1level_ftree)	lal::linarr::MDD_1level<lal::graphs::ftree>;
%template(__MDD_1level_rtree)	lal::linarr::MDD_1level<lal::graphs::rtree>;

%template(__MDD_2level_rational_ugraph)	lal::linarr::MDD_2level_rational<lal::graphs::ugraph>;
%template(__MDD_2level_rational_dgraph)	lal::linarr::MDD_2level_rational<lal::graphs::dgraph>;
%template(__MDD_2level_rational_ftree)	lal::linarr::MDD_2level_rational<lal::graphs::ftree>;
%template(__MDD_2level_rational_rtree)	lal::linarr::MDD_2level_rational<lal::graphs::rtree>;

%template(__MDD_2level_ugraph)	lal::linarr::MDD_2level<lal::graphs::ugraph>;
%template(__MDD_2level_dgraph)	lal::linarr::MDD_2level<lal::graphs::dgraph>;
%template(__MDD_2level_ftree)	lal::linarr::MDD_2level<lal::graphs::ftree>;
%template(__MDD_2level_rtree)	lal::linarr::MDD_2level<lal::graphs::rtree>;

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
