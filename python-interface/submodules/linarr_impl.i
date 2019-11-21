
%{

#include <lal/graphs.hpp>
#include <lal/linarr.hpp>

%}

%include "../lal/linarr/1level.hpp"
%include "../lal/linarr/2level.hpp"
%include "../lal/linarr/C.hpp"
%include "../lal/linarr/D.hpp"
%include "../lal/linarr/headedness.hpp"
%include "../lal/linarr/classification.hpp"

// --------------------------------
// Extendind the function templates

%template(__MDD_1level_rational_ugraph)	lal::linarr::MDD_1level_rational<lal::graphs::ugraph>;
%template(__MDD_1level_rational_dgraph)	lal::linarr::MDD_1level_rational<lal::graphs::dgraph>;
%template(__MDD_1level_rational_utree)	lal::linarr::MDD_1level_rational<lal::graphs::utree>;
%template(__MDD_1level_rational_urtree)	lal::linarr::MDD_1level_rational<lal::graphs::urtree>;
%template(__MDD_1level_rational_dtree)	lal::linarr::MDD_1level_rational<lal::graphs::dtree>;
%template(__MDD_1level_rational_drtree)	lal::linarr::MDD_1level_rational<lal::graphs::drtree>;

%template(__MDD_1level_ugraph)	lal::linarr::MDD_1level<lal::graphs::ugraph>;
%template(__MDD_1level_dgraph)	lal::linarr::MDD_1level<lal::graphs::dgraph>;
%template(__MDD_1level_utree)	lal::linarr::MDD_1level<lal::graphs::utree>;
%template(__MDD_1level_urtree)	lal::linarr::MDD_1level<lal::graphs::urtree>;
%template(__MDD_1level_dtree)	lal::linarr::MDD_1level<lal::graphs::dtree>;
%template(__MDD_1level_drtree)	lal::linarr::MDD_1level<lal::graphs::drtree>;

%template(__MDD_2level_rational_ugraph)	lal::linarr::MDD_2level_rational<lal::graphs::ugraph>;
%template(__MDD_2level_rational_dgraph)	lal::linarr::MDD_2level_rational<lal::graphs::dgraph>;
%template(__MDD_2level_rational_utree)	lal::linarr::MDD_2level_rational<lal::graphs::utree>;
%template(__MDD_2level_rational_urtree)	lal::linarr::MDD_2level_rational<lal::graphs::urtree>;
%template(__MDD_2level_rational_dtree)	lal::linarr::MDD_2level_rational<lal::graphs::dtree>;
%template(__MDD_2level_rational_drtree)	lal::linarr::MDD_2level_rational<lal::graphs::drtree>;

%template(__MDD_2level_ugraph)	lal::linarr::MDD_2level<lal::graphs::ugraph>;
%template(__MDD_2level_dgraph)	lal::linarr::MDD_2level<lal::graphs::dgraph>;
%template(__MDD_2level_utree)	lal::linarr::MDD_2level<lal::graphs::utree>;
%template(__MDD_2level_urtree)	lal::linarr::MDD_2level<lal::graphs::urtree>;
%template(__MDD_2level_dtree)	lal::linarr::MDD_2level<lal::graphs::dtree>;
%template(__MDD_2level_drtree)	lal::linarr::MDD_2level<lal::graphs::drtree>;

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
