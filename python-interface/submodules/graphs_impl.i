
%{

// C++ includes
#include <sstream>

// definitions
#include <lal/definitions.hpp>

// lal includes
#include <lal/graphs.hpp>
#include <lal/graphs/output.hpp>

%}

// --------------
// C++ interfaces

%include stdint.i
%include std_string.i
%include std_vector.i
%include std_pair.i

// -------------------------
// renaming of C++ templates

%include "../lal/definitions.hpp"
namespace std {
	%template(edge) pair<lal::node, lal::node>;
	%template(edge_pair) pair<lal::edge, lal::edge>;
	
	%template(uint64_list) vector<uint64_t>;
	
	%template(edge_list) vector<lal::edge>;
	%template(edge_pair_list) vector<lal::edge_pair>;
	
	%template(list_node_list) vector<vector<lal::node> >;
}

// ----------------
// lal header files

%include "../lal/graphs/graph.hpp"
%include "../lal/graphs/ugraph.hpp"
%include "../lal/graphs/dgraph.hpp"

%include "../lal/graphs/tree.hpp"
%include "../lal/graphs/rtree.hpp"

%include "../lal/graphs/utree.hpp"
%include "../lal/graphs/dtree.hpp"

%include "../lal/graphs/urtree.hpp"
%include "../lal/graphs/drtree.hpp"

%ignore lal::graphs::operator<<;
%include "../lal/graphs/output.hpp"

// -------------------------
// Extendind the C++ classes

%extend lal::graphs::ugraph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::dgraph {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::urtree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
%extend lal::graphs::drtree {
	std::string __str__() const {
		std::ostringstream out;
		out << *$self;
		return out.str();
	}
}
