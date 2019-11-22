
%{

// definitions
#include <lal/definitions.hpp>

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
